/*
 * A017747  Binomial coefficients C(n,83).
 * https://oeis.org/A017747
 *
 * Offset:    83    (a(83) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial coefficient C(n,83) for n ≥ 83
 * Range:     u8 n <= 84, u16 n <= 85, u32 n <= 89, u64 n <= 100,
 *            u128 n <= 136, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 83; below that we return 0. */
    if (n < 83)
        return 0;

    const idx_t K = 83;          /* fixed k value */
    seq_t result = 1;            /* running product for C(n, K) */

    for (idx_t i = 1; i <= K; i++) {
        /* Each step implements result = result * (n-K+i) / i
           in a way that avoids overflow when seq_t is a fixed width. */
        seq_t num = n - K + i;          /* factor to multiply with */

        /* Reduce the denominator by any common factors with the current result */
        seq_t g = gcd((seq_t)i, result);
        result /= g;
        seq_t i_div = (seq_t)i / g;      /* remaining part of the denominator */

        /* Multiply and divide; exact division guaranteed by combinatorial identity */
        result = result * num / i_div;
    }

    return result;
}
