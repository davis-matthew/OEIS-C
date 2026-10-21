/*
 * A231882  Number of maximal 2-independent sets in the planar 3 X n grid
 *          graph.
 * https://oeis.org/A231882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from the *
 *            generating function with recurrence order 9: * a(0)=0, * a(n) =
 *            a(n-1) + 3 a(n-3) + a(n-4) + a(n-6) - 2 a(n-7) - a(n-9) * + r(n)
 *            * * where the inhomogeneous part r(n) is non‑zero only for 1 ≤ n
 *            ≤ 9: * r(1)= 3, r(2)= 1, r(3)= 7, r(4)= -3, r(5...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored (cache_len == next index to compute) */
    static size_t cache_cap = 0;   /* allocated capacity in entries */

    /* convert idx_t to size_t safely */
    size_t idx = (size_t)n;

    if (idx >= cache_len) {
        /* grow cache if necessary */
        if (idx >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap <= idx) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failure; return 0 as sentinel */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute values from cache_len up to idx */
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t val;
            if (i == 0) {
                val = 0;                 /* a(0) = 0 */
            } else {
                seq_t sum = 0;

                /* add positive terms */
                sum += cache[i-1];
                if (i >= 3) sum += 3 * cache[i-3];
                if (i >= 4) sum += cache[i-4];
                if (i >= 6) sum += cache[i-6];

                /* subtract negative terms (ensure i >= corresponding index) */
                if (i >= 7) sum -= 2 * cache[i-7];
                if (i >= 9) sum -= cache[i-9];

                /* add the inhomogeneous r(i) term */
                switch (i) {
                case 1: sum += 3;  break;
                case 2: sum += 1;  break;
                case 3: sum += 7;  break;
                case 4: sum -= 3;  break;
                case 5: sum += 4;  break;
                case 6: sum -= 4;  break;
                case 7: sum -= 1;  break;
                case 8: sum -= 2;  break;
                case 9: sum -= 1;  break;
                default:           /* for i > 9, r(i) = 0 */ break;
                }

                val = sum;
            }
            cache[i] = val;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
