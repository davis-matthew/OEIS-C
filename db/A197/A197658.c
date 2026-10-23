/*
 * A197658  Smallest k such that i^k = i mod n for each i in [0..n-1], or 0 if
 *          no such k exists.
 * https://oeis.org/A197658
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = λ(n)+1 if n is squarefree, otherwise 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (λ(n) is the Carmichael lambda function, i.e. the exponent of the
   multiplicative group modulo n.  For squarefree n, λ(n) = lcm(p-1)
   over all distinct prime divisors p of n.) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* values below offset are irrelevant */

    /* Factor n and check that it is squarefree. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 1)                 /* a factor with exponent > 1 → not squarefree */
            return 0;

    /* Compute lcm of (p-1) for all prime divisors p. */
    seq_t lcm_val = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t rem = (seq_t)primes[i] - 1;
        seq_t g   = gcd(lcm_val, rem);
        lcm_val   = (lcm_val / g) * rem;
    }

    return lcm_val + 1;                 /* λ(n)+1 */
}
