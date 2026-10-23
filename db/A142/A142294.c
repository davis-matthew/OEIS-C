/*
 * A142294  Primes congruent to 5 mod 44.
 * https://oeis.org/A142294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 5 modulo 44
 * Range:     u8 n <= 3, u16 n <= 329, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------------------
 *  Deterministic Miller–Rabin primality test for 64‑bit unsigned integers.
 *  Uses the standard set of bases that is correct for all n < 2^64.
 * ---------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    for (; exp; exp >>= 1)
    {
        if (exp & 1)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const uint64_t small_primes[] = {2ull, 3ull, 5ull, 7ull, 11ull, 13ull,
                                            17ull, 19ull, 23ull, 29ull, 31ull, 37ull};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i)
    {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] = {2ull, 325ull, 9375ull, 28178ull,
                                     450775ull, 9780504ull, 1795265022ull};

    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;

        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ----------------------------------------------------------------------
 *  a(idx_t n) returns the nth prime congruent to 5 modulo 44.
 *  Indexing starts at 1: a(1) = 5.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safety */

    idx_t count = 0;
    uint64_t candidate = 5; /* first candidate, 5 ≡ 5 (mod 44) */

    while (count < n)
    {
        if (is_prime_uint64(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 44; /* next number ≡ 5 (mod 44) */
    }
    return 0; /* unreachable */
}
