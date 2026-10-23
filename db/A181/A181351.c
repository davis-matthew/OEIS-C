/*
 * A181351  Exchange 2 and 5 in the prime factorization of n.
 * https://oeis.org/A181351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p)^e , where f(2)=5, f(5)=2, and f(p)=p for
 *            all other primes
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset starts at 1, but guard just in case */
    uint64_t nn = (uint64_t)n;          /* idx_t is uint64_t */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt); /* factorization: primes[i]^exps[i] */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (primes[i] == (uint64_t)2)
            result *= big_pow((seq_t)5, (seq_t)exps[i]);          /* 5^e */
        else if (primes[i] == (uint64_t)5)
            result *= big_pow((seq_t)2, (seq_t)exps[i]);          /* 2^e */
        else
            result *= big_pow((seq_t)primes[i], (seq_t)exps[i]);  /* p^e */
    }
    return result;
}
