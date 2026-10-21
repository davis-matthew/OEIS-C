/*
 * A077136  Composite numbers n whose proper divisors (excluding 1 and n) are
 *          all of the form p or p+1, with p prime.
 * https://oeis.org/A077136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077136: composite numbers n whose proper divisors d (1<d<n)
   satisfy that either d or d-1 is prime.  The function generates
   terms on demand, caching results for efficiency. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime.h"   /* isPrime(idx_t) */

/* Cached terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;  /* Allocated capacity */
static size_t cache_len  = 0;  /* Number of terms stored */

/* Next integer to test (composite numbers only) */
static uint64_t next_candidate = 4;

/* Ensure cache has room for at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
    if (need > new_cap) new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* Integer square root using binary search (uint64_t safe) */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, return 0 */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t m = next_candidate;

        /* Skip primes */
        if (isPrime(m)) {
            ++next_candidate;
            continue;
        }

        bool ok = true;
        uint64_t sqrt_m = isqrt64(m);

        for (uint64_t d = 2; d <= sqrt_m && ok; ++d) {
            if (m % d == 0) {
                uint64_t e = m / d;          /* complementary divisor */

                /* Check divisor d */
                if (!(isPrime(d) || isPrime(d - 1)))
                    ok = false;
                if (!ok) break;

                /* Check e if distinct */
                if (e != d && !(isPrime(e) || isPrime(e - 1)))
                    ok = false;
            }
        }

        if (ok) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;   /* store the term */
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
