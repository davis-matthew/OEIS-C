/*
 * A225139  Primes from merging of 10 successive digits in the decimal
 *          expansion of the Champernowne constant.
 * https://oeis.org/A225139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by sliding a 10‑digit window over
 *            Champernowne's constant; each 10‑digit number with a non‑zero
 *            leading digit that is prime yields a term. The function memoizes
 *            primes in discovery order
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ----------  64‑bit deterministic Miller–Rabin ------------------- */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1) result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i)
    {
        uint64_t a = small_primes[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Static cache of found primes (memoization) */
    static seq_t *prime_cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static bool initialized  = false;

    /* State while scanning the Champernowne constant */
    static uint64_t cur_num   = 1;          /* current integer being expanded */
    static uint64_t window    = 0;          /* last 10 digits as integer */
    static int      digits_in_window = 0;   /* digits currently in window */

    const uint64_t MOD          = 10000000000ULL; /* 10^10 */
    const uint64_t MIN_WIN      = 1000000000ULL;  /* ensure 10 digits (no leading zero) */

    /* Initialise cache on first call */
    if (!initialized)
    {
        cache_cap = 64;
        prime_cache = seq_alloc(cache_cap);
        cache_size = 0;
        initialized = true;
    }

    /* Generate primes until we have enough for requested index */
    while (cache_size < (size_t)n)
    {
        /* Convert current integer to decimal digits */
        char buf[32];
        int len = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)cur_num);
        for (int i = 0; i < len; ++i)
        {
            int digit = buf[i] - '0';
            window = window * 10 + (uint64_t)digit;
            if (digits_in_window < 10)
            {
                ++digits_in_window;
            }
            else
            {
                window %= MOD;              /* keep only the last 10 digits */
            }

            if (digits_in_window == 10 && window >= MIN_WIN)
            {
                /* We are sure the window has 10 digits and no leading zero */
                if (is_prime_uint64((uint64_t)window))
                {
                    /* Grow cache if needed */
                    if (cache_size == cache_cap)
                    {
                        size_t old_cap = cache_cap;
                        cache_cap *= 2;
                        seq_t *tmp = seq_realloc(prime_cache, old_cap, cache_cap);
                        prime_cache = tmp;
                    }
                    prime_cache[cache_size++] = (seq_t)window;
                }
            }
        }
        ++cur_num;
    }

    return prime_cache[n - 1];
}
