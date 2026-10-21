/*
 * A224516  Number of solutions to x^4 - x == 0 (mod n).
 * https://oeis.org/A224516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 8189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative formula:
   a(p^e) = 4   if p ≡ 1 (mod 3)
   a(p^e) = 2   if p ≡ 2 (mod 3)
   a(3^1) = 2
   a(3^e) = 4 for e > 1
   a(1) = 1
   a(n) = ∏_p a(p^e) over the prime factorisation of n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_simple.h"

/* Return the number of solutions of x^4 - x ≡ 0 (mod n). */
seq_t a(idx_t n)
{
    if (n == 1u)                 /* a(1) = 1 by definition */
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_simple((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;
        if (p == 3ULL)
            term = (e == 1) ? (seq_t)2 : (seq_t)4;
        else
            term = (p % 3 == 1) ? (seq_t)4 : (seq_t)2;
        res *= term;
    }
    return res;
}
