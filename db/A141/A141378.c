/*
 * A141378  a(n) = the smallest positive integer non-coprime to both n and
 *          sigma(n), where sigma(n) is the sum of divisors of n.
 * https://oeis.org/A141378
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of gcd(n,σ(n)) if >1; otherwise
 *            a(n) = spf(n) * spf(σ(n))
 * Range:     u8 n <= 130, u16 n <= 1680, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where σ = sum of divisors function */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

static inline uint64_t spf_u64(uint64_t n)
{
    if (n < 2) return 1;                 /* no prime factor for 0,1 */
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0)
            return p;
    return n;                            /* n itself is prime */
}

/* compute σ(n) using simple factorisation */
static inline uint64_t sigma_u64(uint64_t n)
{
    uint64_t result = 1;
    uint64_t remaining = n;
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            uint64_t term = 1;           /* (p^0 + p^1 + ... + p^k) */
            uint64_t power = 1;          /* p^k */
            while (remaining % p == 0) {
                remaining /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (remaining > 1)                   /* prime factor > sqrt(n) */
        result *= (1 + remaining);
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined below offset */

    uint64_t nn = n;
    uint64_t sig = sigma_u64(nn);
    uint64_t g = gcd_u64(nn, sig);

    if (g > 1) {
        uint64_t spf_g = spf_u64(g);
        return (seq_t)spf_g;
    } else {
        uint64_t spf_n  = spf_u64(nn);
        uint64_t spf_sig = spf_u64(sig);
        return (seq_t)(spf_n * spf_sig);
    }
}
