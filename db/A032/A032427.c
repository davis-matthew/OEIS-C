/*
 * A032427  Coefficients of Jacobi elliptic function c(4,m).
 * https://oeis.org/A032427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(4*n+19) - (24*n+36)*7^(2*n+9) +
 *            (96*n^2+312*n+150)*5^(2*n+9) -
 *            (256*n^3+1344*n^2+1520*n+117)*3^(2*n+9) +
 *            512*n^4+3584*n^3+6208*n^2+1336*n-846) / 196608 Computation uses
 *            CRT with three primes to avoid overflow on 64‑bit seq_t
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow_uint(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = mod_mul(result, b, mod);
        b = mod_mul(b, b, mod);
        exp >>= 1;
    }
    return result;
}

static uint64_t mod_inv(uint64_t a, uint64_t mod)
{
    /* mod is prime, use Fermat */
    return mod_pow_uint(a, mod - 2, mod);
}

seq_t a(idx_t n)
{
    /* primes for CRT (prime > 2 and > 3) */
    const uint64_t primes[3] = {1000000007ULL, 1000000009ULL, 1000000033ULL};
    uint64_t r[3];
    const uint64_t inv_div[3] = {
        mod_inv(196608 % primes[0], primes[0]),
        mod_inv(196608 % primes[1], primes[1]),
        mod_inv(196608 % primes[2], primes[2])
    };

    uint64_t nn = (uint64_t)n;  /* n fits into 64 bits */

    for (int pi = 0; pi < 3; ++pi) {
        uint64_t p = primes[pi];
        /* powers */
        uint64_t pow1 = mod_pow_uint(3, 4 * nn + 19, p);
        uint64_t pow2 = mod_pow_uint(7, 2 * nn + 9, p);
        uint64_t pow3 = mod_pow_uint(5, 2 * nn + 9, p);
        uint64_t pow4 = mod_pow_uint(3, 2 * nn + 9, p);

        /* coefficients modulo p */
        uint64_t c1 = (24 * nn + 36) % p;
        uint64_t c2 = (96 * nn * nn + 312 * nn + 150) % p;
        uint64_t c3 = (256 * nn * nn * nn + 1344 * nn * nn + 1520 * nn + 117) % p;
        uint64_t poly4 = (512 * nn * nn * nn * nn + 3584 * nn * nn * nn + 6208 * nn * nn + 1336 * nn + (p - 846 % p)) % p;

        /* numerator modulo p */
        uint64_t term = pow1 % p;
        term = (term + p - mod_mul(c1, pow2, p)) % p;
        term = (term + mod_mul(c2, pow3, p)) % p;
        term = (term + p - mod_mul(c3, pow4, p)) % p;
        term = (term + poly4) % p;

        r[pi] = mod_mul(term, inv_div[pi], p);
    }

    /* Garner's algorithm for reconstruction */
    __uint128_t x = r[0];
    __uint128_t m = primes[0];
    for (int i = 1; i < 3; ++i) {
        uint64_t mi = primes[i];
        uint64_t mi_inv = mod_inv((uint64_t)(m % mi), mi);
        uint64_t xi = (uint64_t)(x % mi);
        uint64_t t = (r[i] + mi - xi) % mi;
        t = (uint64_t)((__uint128_t)t * mi_inv % mi);
        x += m * t;
        m *= mi;
    }

    return (seq_t)x;
}
