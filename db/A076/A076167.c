/*
 * A076167  Primes p such that sum of even digits of p equals sum of odd
 *          digits of p.
 * https://oeis.org/A076167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 223, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_digit_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate primes, keep those whose even‑digit sum equals
 * the odd‑digit sum.  Results are memoised so repeated calls (in any
 * order of n) are fast.  The implementation uses deterministic
 * Miller‑Rabin for 64‑bit integers.  All computations are performed in
 * uint64_t and the returned value is converted to seq_t. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_digit_sums.h"

/* modular multiplication using 128‑bit intermediate */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation a^e mod m */
static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    a %= m;
    while (e) {
        if (e & 1U) r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
        e >>= 1U;
    }
    return r;
}

/* deterministic Miller‑Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;
    /* small primes up to 37 */
    static const uint64_t trivials[] = {2U,3U,5U,7U,11U,13U,17U,19U,23U,29U,31U,37U};
    for (size_t k = 0U; k < sizeof(trivials)/sizeof(trivials[0]); ++k) {
        if (n == trivials[k]) return true;
        if (n % trivials[k] == 0U) return false;
    }

    /* write n‑1 = d * 2^s */
    uint64_t d = n - 1U;
    unsigned s = 0U;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    /* witnesses sufficient for 64‑bit integers */
    static const uint64_t test_bases[] = {2U, 325U, 9375U, 28178U, 450775U, 9780504U, 1795265022U};
    for (size_t k = 0U; k < sizeof(test_bases)/sizeof(test_bases[0]); ++k) {
        uint64_t a = test_bases[k] % n;
        if (a == 0U) return true;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1U || x == n - 1U) continue;
        bool cont = false;
        for (unsigned r = 1U; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1U) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static cache of found primes satisfying the digit condition */
static seq_t *cache = NULL;   /* array of seq_t values */
static size_t cache_len = 0;  /* number of elements stored */
static size_t cache_cap = 0;  /* allocated capacity */
static uint64_t candidate = 2ULL;   /* next number to test */

/* Main entry function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0U) return 0U;   /* undefined for n < 1, but returns 0 */

    /* generate enough terms if cache is insufficient */
    while (cache_len < (size_t)n) {
        bool prime = false;
        if (candidate == 2U) {
            prime = true;
        } else if ((candidate & 1U) == 0U) {
            prime = false;               /* even number > 2 */
        } else if (is_prime_uint64(candidate)) {
            prime = true;
        }

        if (prime && equal_digit_sums(candidate)) {
            /* allocate more space if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1U : 8U;  /* grow by factor 2, start with 8 */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure: abort by returning 0 */
                    return 0U;
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        /* advance to next candidate */
        if (candidate < 3U) candidate = 3U;
        else              candidate += 2U;  /* skip even numbers */
    }

    return cache[n - 1U];
}
