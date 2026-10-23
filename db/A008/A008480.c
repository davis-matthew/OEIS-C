/*
 * A008480  Number of ordered prime factorizations of n.
 * https://oeis.org/A008480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑k_j)! / ∏k_j! for n = ∏p_j^{k_j} (ordered prime
 *            factorizations)
 * Range:     u8 n <= 2159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* a(1) = 1 */

    /* factor_u64 generates arrays of primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    idx_t sum = 0;         /* running total of exponents seen so far */
    seq_t res = (seq_t)1;  /* multinomial product */

    for (size_t i = 0; i < cnt; ++i) {
        idx_t k = (idx_t)exps[i];
        sum += k;                                  /* new total exponents */
        seq_t choose = binomial(sum, k);           /* C(sum, k) */
        res *= choose;                             /* multiply into result */
    }

    return res;
}
