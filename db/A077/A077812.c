/*
 * A077812  Squarefree kernel of numbers containing in their decimal
 *          representation only the digits 0 and 1.
 * https://oeis.org/A077812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007947( A007088(n) ) A007088(n) : interpret the binary
 *            representation of n as a decimal number containing only digits 0
 *            and 1. A007947(x) : square‑free kernel (product of distinct
 *            prime factors) of x
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities are assumed to be provided in the build
   environment.  The following implementation uses only the
   public API and standard library functions. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* ----------  Helper: Euclidean GCD for uint64_t  ---------- */

/* ----------  Helper: modular multiplication (128‑bit)  ---------- */
static inline uint64_t
mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* ----------  Deterministic Miller–Rabin for 64‑bit  ---------- */
static bool
is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases are sufficient for 64‑bit integers */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = 1, pow = d;
        uint64_t base = a;
        /* modular exponentiation */
        while (pow) {
            if (pow & 1) x = mul_mod(x, base, n);
            base = mul_mod(base, base, n);
            pow >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------  Pollard–Rho factorisation (no static data)  ---------- */
static uint64_t
pollard_rho(uint64_t n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    uint64_t x = 2, y = 2, c = 1, d = 1;
    while (d == 1) {
        x = (mul_mod(x, x, n) + c) % n;
        y = (mul_mod(y, y, n) + c) % n;
        y = (mul_mod(y, y, n) + c) % n;
        uint64_t diff = x > y ? x - y : y - x;
        d = gcd64(diff, n);
        if (d == n) {           /* retry with different constants */
            c++;
            x = 2;
            y = 2;
            d = 1;
        }
    }
    return d;
}

/* ----------  Recursive factorisation into distinct primes  ---------- */
static void
factor_uint64(uint64_t n, uint64_t *fac, int *cnt)
{
    if (n == 1) return;
    if (is_prime_uint64(n)) {
        fac[(*cnt)++] = n;
        return;
    }
    uint64_t d = pollard_rho(n);
    factor_uint64(d, fac, cnt);
    factor_uint64(n / d, fac, cnt);
}

/* ----------  Square‑free kernel (radical) of a 64‑bit integer  ---------- */
static uint64_t
rad_uint64(uint64_t n)
{
    if (n <= 1) return n;

    uint64_t factors[64];
    int cnt = 0;
    factor_uint64(n, factors, &cnt);

    /* Deduplicate and multiply */
    uint64_t rad = 1;
    for (int i = 0; i < cnt; ++i) {
        bool dup = false;
        for (int j = 0; j < i; ++j)
            if (factors[i] == factors[j]) { dup = true; break; }
        if (!dup) rad *= factors[i];
    }
    return rad;
}

/* ----------  Main sequence function  ---------- */
seq_t
a(idx_t n)
{
    /* Build the n‑th 0/1 decimal number (binary n as decimal) */
    uint64_t x = n;
    uint64_t m = 0;
    int hb = -1;                         /* highest set bit index */

    uint64_t t = x;
    while (t > 0) {
        ++hb;
        t >>= 1;
    }

    for (int i = hb; i >= 0; --i) {
        uint64_t bit = (x >> i) & 1ULL;
        m = m * 10 + bit;
    }

    /* Compute its square‑free kernel */
    uint64_t r = rad_uint64(m);

    return (seq_t)r;
}
