/*
 * A069799  The number obtained by reversing the sequence of nonzero exponents
 *          in the prime factorization of n with respect to distinct primes
 *          present, as ordered by their indices.
 * https://oeis.org/A069799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1}^{k} p_i^{ e_{k+1-i} }, where n = Π_{i=1}^k
 *            p_i^{e_i} (p_i prime in ascending order). A069799
 * Range:     u8 n <= 51, u16 n <= 271, u32 n <= 2367, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor.h"

/* Multiply a seq_t base by itself exp times. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    /* defined for n ≥ 1; 0 is unused */
    if (n <= 1)
        return (seq_t)1;

    /* factorize n.  The helper returns primes in ascending order
       and their corresponding exponents. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor((uint64_t)n, primes, exps, &cnt);

    /* reverse the exponents while keeping the primes in order. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        idx_t  e = (idx_t)exps[cnt - 1 - i];
        result *= pow_seq_t(p, e);
    }
    return result;
}
