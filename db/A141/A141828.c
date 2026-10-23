/*
 * A141828  a(n) = (n^4*a(n-1)-1)/(n-1) for n >= 2, with a(0) = 1, a(1) = 5.
 * https://oeis.org/A141828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4*a(n-1)-1)/(n-1), a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- memoisation buffer ---------- */
    static seq_t *buf   = NULL;   /* dynamic array of cached terms   */
    static size_t  cap  = 0;      /* allocated capacity, in elements */
    static size_t  len  = 0;      /* number of elements actually stored */

    /* ---------- grow buffer if necessary ---------- */
    if (n >= (idx_t)len) {
        size_t new_cap = (len == 0) ? 1 : len * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            buf = tmp;
            cap = new_cap;
        }
        /* ---------- compute missing terms ---------- */
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0) {
                buf[0] = (seq_t)1;
            } else if (i == 1) {
                buf[1] = (seq_t)5;
            } else {
                seq_t cur;
                if (sizeof(seq_t) <= 8) {          /* use 128‑bit intermediate */
                    unsigned __int128 k = (unsigned __int128)i;
                    unsigned __int128 prod = (__int128)k * k * k * k *
                                             (__int128)buf[i-1];
                    prod -= 1;
                    cur = (seq_t)(prod / (k-1));
                } else {                           /* arbitrary‑precision path */
                    seq_t k = (seq_t)i;
                    seq_t k4 = k * k * k * k;
                    seq_t tmp = k4 * buf[i-1] - (seq_t)1;
                    cur = tmp / ( (seq_t)(i-1) );
                }
                buf[i] = cur;
            }
        }
        len = n + 1;
    }

    return buf[n];
}
