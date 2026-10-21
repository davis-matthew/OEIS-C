/*
 * A239283  n^(p1) + n^(p2) + n^(p3) + ... where (p1)*(p2)*(p3)*.... is the
 *          prime factorization of n (with multiplicity).
 * https://oeis.org/A239283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i} e_i * n^{p_i}, where n = prod(p_i^{e_i})
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Factorise n: primes[i] ^ exps[i],  i = 0 .. cnt-1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t base = (seq_t)n;
    seq_t sum  = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = big_pow(base, (seq_t)primes[i]);   /* n^p_i */
        term *= (seq_t)exps[i];                        /* multiply by the exponent */
        sum += term;
    }
    return sum;
}
