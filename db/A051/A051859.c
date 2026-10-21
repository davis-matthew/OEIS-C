/*
 * A051859  Values of C (the hypotenuse) of a Pythagorean triangle with A (the
 *          short leg) and C both prime and part of a twin prime.
 * https://oeis.org/A051859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*u(n)^2 − 2*u(n) + 1
 *           where u(n) is the n‑th integer u such that
 *           A = 2u−1 is prime and part of a twin prime pair,
 *           and C = 2u^2−2u+1 is also prime and part of a twin prime pair.
 *           (All values are 1‑based; a(1) is the first listed term.)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* --- a very fast deterministic Miller‑Rabin for 64‑bit integers --- */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    __uint128_t res = ( __uint128_t )a * b;
    return (uint64_t)(res % mod);
}

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1u)
            result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

static inline bool isPrime64(uint64_t n)
{
    if (n < 2u)
        return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u,
        23u, 29u, 31u, 37u, 41u, 43u, 47u
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i)
    {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0u)
            return false;
    }

    /* Miller‑Rabin deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2u, 325u, 9375u, 28178u, 450775u, 9780504u, 1795265022u};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1u) == 0u)
    {
        d >>= 1u;
        ++s;
    }

    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0u)
            continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1u || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul64(x, x, n);
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

/* helper: true iff p is a twin prime (p≥3, and p±2 prime) */
static inline bool is_twin_prime(seq_t p)
{
    if (p < 3u)
        return false;
    /* only need one of p-2 or p+2 to be prime */
    uint64_t pp = (uint64_t)p;
    if (pp > 2u && isPrime64(pp - 2u))
        return true;
    return isPrime64(pp + 2u);
}

/* --- memoised term storage --- */
#define MAX_TERMS 20000
static seq_t terms[MAX_TERMS];
static idx_t terms_len = 0;
static idx_t last_checked_u = 1;

/* Function computing a(n) for n ≥ 1 (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset is 1 – undefined for n<1 */

    /* generate terms until we have at least n of them */
    while (terms_len < n)
    {
        ++last_checked_u;
        seq_t u = (seq_t)last_checked_u;

        /* short leg A = 2u-1, must be prime and a twin prime */
        seq_t A = 2u * u - (seq_t)1;
        if (!isPrime64((uint64_t)A))
            continue;
        if (!is_twin_prime(A))
            continue;

        /* hypotenuse C = 2u^2 − 2u + 1, must be prime and a twin prime */
        seq_t C = 2u * u * u;      /* 2*u*u first */
        C -= 2u * u;               /* subtract 2u   */
        C += (seq_t)1;             /* add 1         */
        if (!isPrime64((uint64_t)C))
            continue;
        if (!is_twin_prime(C))
            continue;

        /* store the term if space remains */
        if (terms_len < MAX_TERMS)
            terms[terms_len++] = C;
        /* otherwise silently ignore further terms – requested n is small */
    }

    return terms[n - 1];
}
