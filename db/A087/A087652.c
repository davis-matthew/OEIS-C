/*
 * A087652  Product of the nonprime divisors of n.
 * https://oeis.org/A087652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the nonprime divisors of n a(n) = 1 if n is 1
 *            or prime a(n) = n^(d(n)/2) / (product of distinct prime divisors
 *            of n), where d(n) is the number of divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 71, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Computes A087652: product of nonprime divisors of n */
seq_t a(idx_t n)
{
    /* n == 1 --> return 1 */
    if (n <= 1)
        return 1;

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute number of divisors d(n) = Π (e_i + 1) */
    idx_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (exps[i] + 1);

    /* product of all divisors */
    seq_t prodAll;
    if ((d & 1) == 0) {                 /* d is even */
        prodAll = big_pow((seq_t)n, (seq_t)(d / 2));
    } else {                            /* d is odd – n is a perfect square */
        /* n^(d-1)/2 part */
        seq_t temp = big_pow((seq_t)n, (seq_t)((d - 1) / 2));

        /* sqrt(n) = Π p_i^(e_i/2) */
        seq_t sqrt_n = 1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p_pow = big_pow((seq_t)primes[i], (seq_t)(exps[i] / 2));
            sqrt_n = sqrt_n * p_pow;
        }
        prodAll = temp * sqrt_n;
    }

    /* product of all distinct prime divisors */
    seq_t prodPrimes = 1;
    for (size_t i = 0; i < cnt; ++i)
        prodPrimes *= (seq_t)primes[i];

    /* final result: cancel prime divisors from product of all divisors */
    return prodAll / prodPrimes;
}
