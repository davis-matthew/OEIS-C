/*
 * A234406  Number of (n+1)X(1+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 12
 *          and no adjacent elements equal.
 * https://oeis.org/A234406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n‑1) +20*a(n‑2) -301*a(n‑3) -44*a(n‑4) +2831*a(n‑5)
 *            -301*a(n‑6) -8624*a(n‑7) -3046*a(n‑8) +4932*a(n‑9) +2352*a(n‑10)
 *            with initial values a(1)=300, a(2)=2540, a(3)=21288,
 *            a(4)=185000, a(5)=1604528, a(6)=14012832, a(7)=122325680,
 *            a(8)=1069226864, a(9)=9345048848, a(10)=81695419072
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define CACHE_MAX 20000                      /* enough for n up to ~10⁴ */

static seq_t cache[CACHE_MAX + 1];           /* 1‑based: cache[1] == a(1) */
static idx_t cache_len = 0;                  /* largest index stored */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined, guard */
    if (n > (idx_t)CACHE_MAX) return (seq_t)0;/* out of range guard */

    /* initialise the first ten terms once */
    if (cache_len < 1) {
        cache[1] = (seq_t)300;
        cache[2] = (seq_t)2540;
        cache[3] = (seq_t)21288;
        cache[4] = (seq_t)185000;
        cache[5] = (seq_t)1604528;
        cache[6] = (seq_t)14012832;
        cache[7] = (seq_t)122325680;
        cache[8] = (seq_t)1069226864;
        cache[9] = (seq_t)9345048848;
        cache[10] = (seq_t)81695419072;
        cache_len = 10;
    }

    /* compute any missing terms up to n */
    if (cache_len < n) {
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t pos = (seq_t)10 * cache[k - 1] + (seq_t)20 * cache[k - 2]
                        + (seq_t)2831 * cache[k - 5]
                        + (seq_t)4932 * cache[k - 9]
                        + (seq_t)2352 * cache[k - 10];

            seq_t neg = (seq_t)301 * cache[k - 3] + (seq_t)44 * cache[k - 4]
                        + (seq_t)301 * cache[k - 6]
                        + (seq_t)8624 * cache[k - 7]
                        + (seq_t)3046 * cache[k - 8];

            cache[k] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n];
}
