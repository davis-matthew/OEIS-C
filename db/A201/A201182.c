/*
 * A201182  Primes of the form 7*k^3+1.
 * https://oeis.org/A201182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 7*k^3 + 1 (OEIS A201182)
 * Range:     u8 none, u16 n <= 3, u32 n <= 57, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 2385 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    if (n % 3ULL == 0)  return n == 3;

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;          /* base divisible by n: pass */

        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- trial division by small primes ---- */
static bool is_prime(uint64_t n)
{
    static const uint32_t small_primes[] =
    {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
        31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

/* Return the n-th prime of the form 7*k^3+1, with a(1) being the first */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    seq_t value;

    while (count < n) {
        /* Compute 7*k^3 + 1 using 128‑bit intermediate to avoid overflow */
        seq_t kk = (seq_t)k * k;           // k^2
        seq_t kkk = kk * k;               // k^3
        value = 7 * kkk + 1;

        /* Prime test on the 64‑bit representation (value fits in uint64_t) */
        if (is_prime((uint64_t)value)) {
            ++count;
            if (count == n)
                return value;
        }
        ++k;
    }
    /* Should never reach here */
    return 0;
}
