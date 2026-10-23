/*
 * A035148  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -42.
 * https://oeis.org/A035148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-42,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod64.h"


static int kronecker_minus42_prime(uint64_t p)
{
    /* symbols for primes dividing 42 */
    if (p == 2 || p == 3 || p == 7) return 0;

    /* Jacobi(-42 | p) = (-42)^((p-1)/2) mod p   */
    uint64_t a = p - (42 % p);          /* (-42) mod p, non‑zero because p∤42 */
    if (a == p) a = 0;                  /* safety, not needed for p∤42 */

    uint64_t res = powmod64(a, (p - 1) >> 1, p);
    if (res == 1)        return 1;
    if (res == p - 1)    return -1;
    return 0;                                 /* should not occur for odd prime */
}

static seq_t term_for_prime_power(uint64_t p, unsigned int e)
{
    int sym = kronecker_minus42_prime(p);

    if (sym == 0)                 /* p | 42 */
        return 1;
    if (sym == -1)                /* (-42|p) = -1 */
        return (e & 1) ? 0 : 1;   /* (1+(-1)^e)/2 */
    /* sym == 1 */
    return (seq_t)(e + 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, but never used */

    uint64_t val = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64(val, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= term_for_prime_power(pr[i], (unsigned int)ex[i]);

    return result;
}
