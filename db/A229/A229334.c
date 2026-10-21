/*
 * A229334  Product of numbers of elements of nonempty subsets of divisors of
 *          n.
 * https://oeis.org/A229334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..τ(n)} k^{C(τ(n),k)} , where τ(n) = number of
 *            divisors of n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binom, divisor_count_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses helper functions supplied by the build:
 * - divisor_count_u64 : returns τ(n) for a uint64_t value.
 * - binom               : binomial coefficient C(m,k) as seq_t.
 * - big_pow             : computes base^exp for arbitrary‑precision seq_t.
 * - seq_mul             : multiplies two seq_t values.
 */
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/binom.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1 – not used, but safe */

    /* τ(n) – number of divisors of n */
    idx_t tau = divisor_count_u64((uint64_t)n);

    seq_t result = 1;
    for (idx_t k = 1; k <= tau; ++k) {
        /* exponent = C(tau, k) */
        seq_t exp = binom(tau, k);

        /* term = k ^ exp */
        seq_t term = big_pow((seq_t)k, exp);

        /* multiply into result */
        result = seq_mul(result, term);
    }
    return result;
}
