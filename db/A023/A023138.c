/*
 * A023138  Number of cycles of function f(x) = 6x mod n.
 * https://oeis.org/A023138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/ord(6,m)) * Σ_{j=0}^{ord(6,m)-1} gcd(6^j-1, m), where
 *            m = n with all factors of 2 and 3 removed
 * Range:     u8 n <= 1294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* numeric helpers -------------------------------------------------------- */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
/* modular exponentiation: (base^exp) mod mod, using 128-bit intermediate. */
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

static uint64_t phi_uint64(uint64_t x)
/* Euler's totient for 64‑bit unsigned integers. */
{
    uint64_t result = x;
    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result = result / p * (p - 1);
        }
    }
    if (n > 1)
        result = result / n * (n - 1);
    return result;
}

static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *factors, size_t *cnt)
/* Gives prime factors `primes` and their exponents `factors` of n.
   The caller supplies arrays large enough for the result. */
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            primes[(*cnt)]   = p;
            factors[(*cnt)]  = exp;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[(*cnt)]   = n;
        factors[(*cnt)]  = 1;
        ++(*cnt);
    }
}

/* sequence generator ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined, but protect */
        return (seq_t)0;
    }

    uint64_t nn = (uint64_t)n;
    uint64_t m = nn;
    while ((m & 1) == 0) m >>= 1;                /* remove factors of 2 */
    while (m % 3 == 0) m /= 3;                   /* remove factors of 3 */

    if (m == 1)
        return (seq_t)1;                         /* special case */

    /* compute ord(6, m) */
    uint64_t ord = phi_uint64(m);
    uint64_t primes[64], exps[64];
    size_t pf_count;
    factor_uint64(ord, primes, exps, &pf_count);

    for (size_t i = 0; i < pf_count; ++i) {
        uint64_t p = primes[i];
        while (ord % p == 0 &&
               powmod_uint64(6, ord / p, m) == 1)
            ord /= p;
    }

    /* sum gcd(6^j - 1, m) for j = 0 .. ord-1 */
    uint64_t sum = 0;
    uint64_t rem = 1 % m;          /* 6^0 mod m */
    for (uint64_t j = 0; j < ord; ++j) {
        uint64_t diff = (rem + m - 1) % m;      /* (6^j - 1) mod m */
        sum += gcd_uint64(diff, m);
        rem = rem * 6 % m;                      /* next power of 6 */
    }

    seq_t result = (seq_t)(sum / ord);
    return result;
}
