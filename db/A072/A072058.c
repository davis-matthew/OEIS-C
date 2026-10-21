/*
 * A072058  Squarefree kernel of 2*prime(n)+1.
 * https://oeis.org/A072058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree kernel of 2*prime(n)+1
 * Range:     u8 n <= 31, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* offset is 1, n>=1 */
    uint32_t p = get_prime(n);                     /* nth prime, 1‑indexed */
    seq_t m = (seq_t)2 * (seq_t)p + 1;             /* 2*prime(n)+1 */

    /* factor m – it is < 2·10^5 for n≤10000, so a few prime factors suffice */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* product of distinct prime factors = radical of m */
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
        res *= (seq_t)primes[i];

    return res;
}
