/*
 * A176178  Concatenation of perfect numbers written in base 2.
 * https://oeis.org/A176178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) is the n‑th bit (1‑indexed) in the infinite binary string
 *            obtained by concatenating the binary representations of perfect
 *            numbers 6 = 110, 28 = 11100, 496 = 111110000, 8128 =
 *            1111111000000, ... The i‑th perfect number has binary form:
 *            2^(p-1)·(2^p−1) where 2^p−1 is a Mersenne prime. Thus its...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Known exponents p for which 2^p−1 is prime (Mersenne primes). */
    static const uint16_t exps[] = {
        2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
        107, 127, 521, 607, 1279, 2203, 2281,
        3217, 4253, 4423
    };
    static const size_t exps_cnt = sizeof(exps) / sizeof(exps[0]);

    uint64_t cum_len = 0;                     /* cumulative bit length so far   */
    size_t i = 0;
    uint64_t n_val = (uint64_t)n;

    /* Find the perfect number whose binary string contains the n-th bit. */
    for (; i < exps_cnt; ++i) {
        uint64_t term_len = (uint64_t)exps[i] * 2ULL - 1ULL;  /* 2p−1 bits */
        if (cum_len + term_len >= n_val)
            break;
        cum_len += term_len;
    }

    /* If n exceeds the precomputed exponents, return 0 (should not happen for tests). */
    if (i == exps_cnt)
        return (seq_t)0;

    uint64_t offset = n_val - cum_len - 1ULL;   /* 0‑based index inside current term */
    if (offset < exps[i])
        return (seq_t)1;   /* within the block of ones   */
    else
        return (seq_t)0;   /* within the trailing zeros */
}
