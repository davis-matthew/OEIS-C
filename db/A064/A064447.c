/*
 * A064447  a(n) = phi(n^n).
 * https://oeis.org/A064447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n^n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Euler totient of n raised to the nth power. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Factor n.  For n ≤ 10 000 the needed storage is trivial. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    /* For each distinct prime factor p^e of n, contribute
       p^(e*n−1)·(p−1).  This follows from
          φ(p^{e*n}) = p^{e*n−1}·(p−1).       */
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t exp = (seq_t)exps[i] * (seq_t)n;   /* e*n */
        exp -= 1;                                 /* e*n − 1 */

        seq_t term = big_pow(p, exp);             /* p^(e*n−1)      */
        term = term * (seq_t)(primes[i] - 1);     /* ⋅ (p−1)        */

        result = result * term;                   /* accumulate    */
    }

    return result;
}
