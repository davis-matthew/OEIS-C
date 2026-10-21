/*
 * A216677  Number of squares s^2 that divide the product of the divisors of
 *          n, such that rad(s) = rad(n), where rad(n) = A007947.
 * https://oeis.org/A216677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏₁≤i≤k ⌊ a_i·(a_i+1)·∏_{j≠i}(1+a_j) / 4 ⌋ where n =
 *            ∏₁≤i≤k p_i^{a_i} is the prime factorisation of n. For n = 1,
 *            a(1) = 1. (See OEIS A216677.)
 * Range:     u8 n <= 179, u16 n <= 2519, u32 n <= 8593, u64 n <= 8593,
 *            u128 n <= 8593, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */

#include "../utils/factor_u64.h"   /* factorisation helper */

/* the types seq_t and idx_t are supplied by the build system */

seq_t a(idx_t n)
{
    /* special case n = 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* --- factorisation of n --- */
    /* maximum 64 distinct primes for uint64_t */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute Tot = ∏(1 + a_i) */
    seq_t Tot = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        Tot *= (seq_t)(exps[i] + 1);

    /* compute the product of the floor‑terms */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* product of (1+a_j) over j≠i : Tot / (1+exps[i])  (exact division) */
        seq_t prod = Tot / (seq_t)(exps[i] + 1);

        /* term = floor( a_i * (a_i + 1) * prod / 4 ) */
        seq_t term = ((seq_t)exps[i] * (seq_t)(exps[i] + 1) * prod) / (seq_t)4;

        result *= term;
    }

    return result;
}
