/*
 * A152171  G.f. := (1-sqrt(1-4*x+4*x^2-4*x^3))/(2(1-x+x^2)x).
 * https://oeis.org/A152171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the D-finite recurrence (n+1)*a(n)
 *            +(-5*n+1)*a(n-1) +9*(n-1)*a(n-2) +12*(-n+2)*a(n-3)
 *            +2*(4*n-11)*a(n-4) +2*(-2*n+7)*a(n-5)=0 with a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=5
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to allow arbitrary call order. */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* next index to compute in cache */
    static size_t cap = 0;   /* allocated capacity of cache */

    /* Grow cache to accommodate up to n. */
    if ((size_t)n + 1 > len) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap ? cap * 2 : 16;
            if (newcap < need) newcap = need;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap   = newcap;
        }

        /* Compute missing terms. */
        for (size_t i = len; i < need; ++i) {
            if (i <= 4) {
                /* Initial values a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=5. */
                static const seq_t base[5] = { (seq_t)1, (seq_t)1, (seq_t)1,
                                              (seq_t)2, (seq_t)5 };
                cache[i] = base[i];
            } else {
#ifdef __cplusplus
                /* C++ build: use seq_t arithmetic (arbitrary‑precision). */
                seq_t num = 0;
                num += (seq_t)(5 * (idx_t)i - 1) * cache[i - 1];
                num += (seq_t)(12 * (idx_t)i - 24) * cache[i - 3];
                num += (seq_t)(4 * (idx_t)i - 14) * cache[i - 5];
                num -= (seq_t)(9 * (idx_t)i - 9)  * cache[i - 2];
                num -= (seq_t)(8 * (idx_t)i - 22) * cache[i - 4];
                cache[i] = num / (seq_t)((idx_t)i + 1);
#else
                /* C build: use unsigned __int128 for intermediate safety. */
                unsigned __int128 num = 0;
                /* Positive contributions */
                num += (unsigned __int128)(5 * i - 1) *
                       (unsigned __int128)cache[i - 1];
                num += (unsigned __int128)(12 * i - 24) *
                       (unsigned __int128)cache[i - 3];
                num += (unsigned __int128)(4 * i - 14) *
                       (unsigned __int128)cache[i - 5];
                /* Negative contributions */
                num -= (unsigned __int128)(9 * i - 9) *
                       (unsigned __int128)cache[i - 2];
                num -= (unsigned __int128)(8 * i - 22) *
                       (unsigned __int128)cache[i - 4];
                cache[i] = (seq_t)(num / (unsigned __int128)(i + 1));
#endif
            }
        }
        len = need;
    }
    return cache[(size_t)n];
}
