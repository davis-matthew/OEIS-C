/*
 * A089233  Number of coprime pairs of divisors > 1 of n.
 * https://oeis.org/A089233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p^e‖n} (1+2e) – 2·τ(n) + 1)/2 where τ(n) = ∏(e+1) is
 *            the number of divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Treat n=1 specially – it has no divisors >1 */
    if (n == 1) {
        return 0;
    }

    /* Small fixed buffer for prime factors (max 64 on 64‑bit n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {          /* Remaining prime factor */
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Compute the product and τ(n) in 128‑bit arithmetic */
    unsigned __int128 prod = 1;
    unsigned __int128 tau  = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        prod *= (unsigned __int128)(1 + 2 * e);
        tau  *= (unsigned __int128)(e + 1);
    }

    unsigned __int128 numerator = prod - 2 * tau + 1;
    unsigned __int128 result    = numerator / 2;

    return (seq_t)result;
}
