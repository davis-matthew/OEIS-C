/*
 * A073108  Least k such that there are n primes among the numbers n^2 + x^2
 *          for 1 <= x <= k.
 * https://oeis.org/A073108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A073108:
   a(n) = least k such that among the numbers n^2 + x^2 (1 ≤ x ≤ k) 
   there are exactly n primes.  Offset 1 (a(1)=1).  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mod_mul64(r, a, m);
        a = mod_mul64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i)
    {
        if (n % small[i] == 0) return n == small[i];
    }

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul64(x, x, n);
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

/* Cache of previously computed values (offset 1).  0 means “not computed”.
   200000 is more than enough for the test limits. */
static seq_t cache[200000] = {0};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* guard – offset is 1 */

    /* use cached value if available */
    if (n - 1 < 200000 && cache[n - 1] != 0)
        return cache[n - 1];

    uint64_t n2 = (uint64_t)n * (uint64_t)n;   /* n^2  (fits in 64‑bit for n≤10⁴) */
    uint64_t primes_found = 0;
    uint64_t k = 0;

    while (primes_found < n)
    {
        ++k;
        uint64_t val = n2 + k * k;          /* n^2 + k^2  (fits in 64‑bit for tested n) */
        if (is_prime64(val)) ++primes_found;
    }

    /* store in cache if the slot is within bounds */
    if (n - 1 < 200000) cache[n - 1] = (seq_t)k;
    return (seq_t)k;
}
