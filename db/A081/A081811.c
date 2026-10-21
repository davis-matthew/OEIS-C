/*
 * A081811  If n = p_1^e_1 * ... * p_k^e_k, p_1 < ... < p_k primes, then a(n)
 *          = min{ p_i*e_i }.
 * https://oeis.org/A081811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = min{ p_i * e_i } where n = p_1^e_1 *
 *            … * p_k^e_k (p_i are distinct primes)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides void factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */

seq_t a(idx_t n)
{
    /* the sequence is only defined for n ≥ 1 */
    if (n <= 1U)
        return (seq_t)0;

    /* arrays to hold prime factors and their exponents.
       A 64‑bit integer can have at most ~16 distinct prime factors,
       so 64 is more than sufficient. */
    static uint64_t pr[64];
    static uint64_t exp[64];
    size_t cnt;

    /* factor the input number */
    factor_u64((uint64_t)n, pr, exp, &cnt);

    /* initialise the minimum with the first factor's product */
    seq_t min_val = (seq_t)(pr[0] * exp[0]);

    /* iterate over the remaining factors */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t val = (seq_t)(pr[i] * exp[i]);
        if (val < min_val)
            min_val = val;
    }

    return min_val;
}
