/*
 * A063514  a(n) = sigma(n) mod phi(n).
 * https://oeis.org/A063514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ(n) mod φ(n) (σ = sum‑of‑divisors, φ = totient)
 * Range:     u8 n <= 474, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for n >= 1.  For n < 1 the return value is undefined
   but a 0 is returned to keep the function safe.                         */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0u;            /* undefined offset – keep it harmless */

    if (n == 1)
        return 0u;            /* σ(1)=1, φ(1)=1 ⇒ 0 */

    /* --- Factor n by simple trial division --------------------------------*/
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    unsigned int exps[64];
    unsigned int cnt = 0;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (p > 2 && (p & 1) == 0)          /* skip even numbers after 2 */
            continue;
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1) {                            /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* --- Compute φ(n)  (intermediate in 128‑bit, final < n) ---------------*/
    unsigned __int128 phi128 = 1;
    for (unsigned int i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        unsigned int e = exps[i];
        phi128 *= (p - 1);
        for (unsigned int k = 1; k < e; ++k)
            phi128 *= p;
    }
    uint64_t phi = (uint64_t)phi128;         /* φ(n) ≤ n ≤ 2⁶⁴‑1, fits in 64‑bit */

    /* --- Compute σ(n) modulo φ(n) ---------------------------------------*/
    unsigned __int128 sigma_mod = 1;          /* product of (1+p+…+pᵉ) */
    for (unsigned int i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        unsigned int e = exps[i];

        unsigned __int128 sum = 1;            /* Σ pᵏ, k=0..e */
        unsigned __int128 pow = 1;
        for (unsigned int k = 0; k < e; ++k) {
            pow = (pow * p) % phi;
            sum = (sum + pow) % phi;
        }
        sigma_mod = (sigma_mod * sum) % phi;
    }

    /* σ(n) modulo φ(n) is already reduced to [0,φ-1] */
    return (seq_t)(sigma_mod % phi);
}
