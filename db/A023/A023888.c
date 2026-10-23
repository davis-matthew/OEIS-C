/*
 * A023888  Sum of prime power divisors of n (1 included).
 * https://oeis.org/A023888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all prime power divisors of n, 1 included. a(1) =
 *            1 a(p^k) = (p^(k+1)-1)/(p-1) for prime p and integer k>=1 a(n)
 *            for composite n = sum_{p|n} (p^(e_p+1)-1)/(p-1) - (omega(n)-1)
 *            (1 counted once)
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case for n == 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factorise n: primes[], exps[] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t term = 0;
        seq_t cur  = 1;
        for (uint64_t e = 0; e <= exps[i]; ++e) {
            term += cur;   /* add p^e */
            cur  *= p;
        }
        sum += term;
    }

    /* Subtract duplications of divisor 1 for each additional prime */
    sum -= (seq_t)(cnt - 1);
    return sum;
}
