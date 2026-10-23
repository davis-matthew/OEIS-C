/*
 * A105073  Define a(1)=0, a(2)=2 then a(n) = 3*a(n-1) - a(n-2), a(n+1) =
 *          3*a(n)-a(n-1) and a(n+2) = 3*a(n+1) - a(n) + 2.
 * https://oeis.org/A105073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=2, a(3)=6, a(4)=16, a(5)=44; a(n) = 3*a(n-1) –
 *            a(n-2) + a(n-3) – 3*a(n-4) + a(n-5) for n ≥ 6
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence defined only for n ≥ 1 */

    /* 1‑based memoisation table: buf[1] is a(1) */
    static seq_t *buf = NULL;
    static size_t cap = 0;   /* buffer allocation size */
    static size_t sz  = 0;   /* largest index computed so far */

    /* grow buffer if needed */
    if (n + 1 > cap) {
        size_t newcap = (cap == 0) ? 16 : cap * 2;
        while (newcap < n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, newcap * sizeof(seq_t));
        buf = tmp;
        cap = newcap;
    }

    /* initialise the first five terms once */
    if (sz == 0) {
        buf[0] = 0;   /* unused slot for convenience */
        buf[1] = 0;
        buf[2] = 2;
        buf[3] = 6;
        buf[4] = 16;
        buf[5] = 44;
        sz = 5;
    } else if (sz < 5) {      /* in case the array was resized later */
        for (size_t i = sz + 1; i <= 5; ++i) {
            switch (i) {
                case 1: buf[1] = 0; break;
                case 2: buf[2] = 2; break;
                case 3: buf[3] = 6; break;
                case 4: buf[4] = 16; break;
                case 5: buf[5] = 44; break;
            }
        }
        sz = 5;
    }

    /* compute missing terms up to n */
    for (idx_t i = (sz < 5 ? 6 : sz + 1); i <= n; ++i) {
        seq_t t1 = buf[i-1] * (seq_t)3;
        seq_t t2 = buf[i-2];
        seq_t t3 = buf[i-3];
        seq_t t4 = buf[i-4] * (seq_t)3;
        seq_t t5 = buf[i-5];
        buf[i] = t1 - t2 + t3 - t4 + t5;
    }

    sz = n;  /* update largest computed index */
    return buf[n];
}
