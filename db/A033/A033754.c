/*
 * A033754  Expansion of Product_{d | 42} theta_3(q^d).
 * https://oeis.org/A033754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|42} θ₃(q^d)
 * Range:     u8 n <= 26, u16 n <= 182, u32 n <= 6887, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *coeff_cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < cache_len && coeff_cache != NULL)
        return coeff_cache[n];

    idx_t new_len = n + 1;
    seq_t *cur = (seq_t *)calloc((size_t)new_len, sizeof(seq_t));
    cur[0] = 1;

    /* Divisors of 42 */
    const idx_t divisors[] = {1, 2, 3, 6, 7, 14, 21, 42};
    const size_t dcnt = 8;

    for (size_t di = 0; di < dcnt; ++di) {
        idx_t d = divisors[di];
        seq_t *next = (seq_t *)calloc((size_t)new_len, sizeof(seq_t));

        for (idx_t sum = 0; sum < new_len; ++sum) {
            seq_t val = cur[sum];

            /* k = 0 term */
            next[sum] += val;

            /* k > 0 terms */
            for (idx_t k = 1; ; ++k) {
                idx_t exp = d * k * k;
                if (exp > sum) break;
                next[sum] += cur[sum - exp] * 2;
            }
        }

        free(cur);
        cur = next;
    }

    /* Store computed coefficients for future calls */
    free(coeff_cache);
    coeff_cache = cur;
    cache_len = new_len;

    return coeff_cache[n];
}
