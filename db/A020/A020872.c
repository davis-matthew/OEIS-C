/*
 * A020872  Number of restricted forests in Moebius ladder M_n.
 * https://oeis.org/A020872
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 34*a(n-2) + 23*a(n-3) + 43*a(n-4) - 56*a(n-5)
 *            - 3*a(n-6) + 25*a(n-7) - 6*a(n-8) - 3*a(n-9) + a(n-10); Initial
 *            values: a(2)=34, a(3)=241, a(4)=1582, a(5)=10204, a(6)=65197,
 *            a(7)=415076, a(8)=2638366, a(9)=16759249, a(10)=106427154,
 *            a(11)=675771276
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (behaviour below the offset does not matter) */
    if (n < 2)
        return (seq_t)0;

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;          /* array of a[0 .. max_computed] */
    static idx_t max_computed = 1;       /* largest index that has been filled (>=1) */

    /* Grow cache if necessary */
    if (n >= max_computed) {
        size_t new_len = (size_t)(n + 1);                 /* length in terms of indexes 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                               /* abort on allocation failure */
        cache = tmp;
        /* Zero‑initialize new slots */
        if (new_len > (size_t)(max_computed + 1))
            memset((void *)(cache + max_computed + 1),
                   0,
                   (new_len - (max_computed + 1)) * sizeof(seq_t));
    }

    /* Now ensure all terms up to n are computed */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 11) {          /* initial segment, use explicit constants */
            switch (i) {
                case 2: cache[i] = (seq_t)34;           break;
                case 3: cache[i] = (seq_t)241;          break;
                case 4: cache[i] = (seq_t)1582;         break;
                case 5: cache[i] = (seq_t)10204;        break;
                case 6: cache[i] = (seq_t)65197;        break;
                case 7: cache[i] = (seq_t)415076;       break;
                case 8: cache[i] = (seq_t)2638366;      break;
                case 9: cache[i] = (seq_t)16759249;     break;
                case 10: cache[i] = (seq_t)106427154;   break;
                case 11: cache[i] = (seq_t)675771276;   break;
                default: /* unreachable */              break;
            }
        } else {                /* recurrence (10th order linear) */
            seq_t v = 11 * cache[i - 1];
            v += 23 * cache[i - 3];
            v += 43 * cache[i - 4];
            v += 25 * cache[i - 7];
            v += cache[i - 10];

            v -= 34 * cache[i - 2];
            v -= 56 * cache[i - 5];
            v -= 3 * cache[i - 6];
            v -= 6 * cache[i - 8];
            v -= 3 * cache[i - 9];

            cache[i] = v;
        }
    }
    max_computed = n;
    return cache[n];
}
