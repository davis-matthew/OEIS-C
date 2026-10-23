/*
 * A186099  Sum of divisors of n congruent to 1 or 5 mod 6.
 * https://oeis.org/A186099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d|n, d ≡ 1 or 5 (mod 6)} d
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* Helper: compute the sum of divisors of n that are ≡1 or 5 mod 6. */
static seq_t sum_congruent_1_5_mod6(idx_t n)
{
    /* Factor n using the provided helper.  No more than 32 distinct primes will occur for n ≤ 10⁶. */
    uint64_t pr[32];
    uint64_t ex[32];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* Compute the total number of divisors to pre‐allocate an array. */
    size_t div_cnt = 1;
    for (size_t i = 0; i < cnt; ++i)
        div_cnt *= (ex[i] + 1);

    /* Create an array to hold all divisors. */
    seq_t *divs = (seq_t *)malloc(sizeof(seq_t) * div_cnt);
    if (!divs) return 0;   /* safety, though unlikely */

    divs[0] = 1;
    size_t current_cnt = 1;

    /* Generate all divisors iteratively. */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        uint64_t exp = ex[i];
        seq_t *new_divs = (seq_t *)malloc(sizeof(seq_t) * div_cnt);
        if (!new_divs) {
            free(divs);
            return 0;
        }
        size_t new_cnt = 0;
        seq_t p_pow = 1;
        for (uint64_t k = 0; k <= exp; ++k) {
            for (size_t j = 0; j < current_cnt; ++j) {
                new_divs[new_cnt++] = divs[j] * p_pow;
            }
            p_pow *= p;
        }
        free(divs);
        divs = new_divs;
        current_cnt = new_cnt;
    }

    /* Sum divisors that are 1 or 5 mod 6. */
    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t d = divs[i];
        if ((d % 6) == 1 || (d % 6) == 5)
            sum += d;
    }

    free(divs);
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* OEIS offset is 1, so a(1) is the first term. */
    if (n < 1)
        return 0; /* undefined, return 0 */
    return sum_congruent_1_5_mod6(n);
}
