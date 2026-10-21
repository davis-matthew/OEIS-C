/*
 * A082872  a^n + b^n + c^n + ..., where a*b*c* ... is the prime factorization
 *          of n.
 * https://oeis.org/A082872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of p^n for each prime factor p of n, counting
 *            multiplicity
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Example: a(8) = 2^8 + 2^8 + 2^8 = 768 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 explicitly (prime factorization is empty) */
    if (n == 1)
        return (seq_t)1;

    /* Buffer for prime factors and their exponents. 64 is enough for any 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n; factor_u64 writes the primes into primes[], exponents into exps[],
       and the number of distinct primes into cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    seq_t exp_n = (seq_t)n;          /* n as a seq_t for the exponent */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = big_pow((seq_t)primes[i], exp_n);   /* p^n */

        /* Multiply by the exponent (multiplicity).  The exponent fits in a native
           integer; cast to seq_t before multiplication to keep types consistent. */
        term *= (seq_t)exps[i];

        sum += term;                     /* accumulate the result */
    }

    return sum;
}
