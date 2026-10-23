/*
 * A201256  Primes of the form 7k^3+8.
 * https://oeis.org/A201256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 7*k^3 + 8 Construct candidates p =
 *            7*k^3 + 8 for k = 1,2,3,... Test each p for primality
 *            (deterministic Miller–Rabin for 64‑bit). Return the n‑th such
 *            prime. Counts start at n = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 54, u64 n <= 2800,
 *            u128 n <= 2800, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"

/* ---------- Miller–Rabin helpers for 64‑bit integers ---------- */

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1ULL) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1ULL;
    }
    return r;
}

/* Deterministic Miller–Rabin test for 64‑bit unsigned integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime trial division */
    static const uint64_t small[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
         23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0ULL) return false;
    }

    /* Express n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) return true;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1ULL || x == n - 1ULL) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1ULL) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, return zero */

    idx_t found = 0;
    uint64_t k = 1;      /* k fits in 64 bits for all relevant n */
    while (true) {
        /* Compute candidate p = 7*k^3 + 8 safely in 128 bits */
        unsigned __int128 tmp = (unsigned __int128)7 * k * k * k + 8;
        /* For n up to 10000 p < 2^64, so cast is safe for primality test */
        uint64_t p = (uint64_t)tmp;

        if (is_prime_uint64(p)) {
            ++found;
            if (found == n) return (seq_t)p;
        }
        ++k;
    }
}
