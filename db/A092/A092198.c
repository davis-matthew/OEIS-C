/*
 * A092198  Equal count of primes congruent to 1 mod 4 and 3 mod 4 associated
 *          with primes in A007351 (the zero beginning the sequence indicates
 *          the prime 2).
 * https://oeis.org/A092198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the common count of primes ≡1 (mod 4) and ≡3 (mod 4) at
 *            the nth time these running totals are equal, with a(1) = 0. This
 *            function generates primes on demand, tracks the difference of
 *            the two counts, and records each equality
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms: a[1] = 0, a[2] = 1, ... */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of terms stored (≤ cache_cap) */
    static size_t cache_cap = 0;     /* capacity of cache                */

    /* table of primes generated so far */
    static uint64_t *primes = NULL;
    static size_t primes_size = 0;
    static size_t primes_cap = 0;

    /* state maintained between calls */
    static bool initialized = false;
    static int64_t diff = 0;          /* count1 - count3                */
    static seq_t  count1 = 0;         /* primes ≡1 mod 4                */
    static seq_t  count3 = 0;         /* primes ≡3 mod 4                */
    static uint64_t next_candidate = 3; /* next odd number to test */

    /* initialisation on first call */
    if (!initialized) {
        /* cache: at least one element for a(1) */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)0;        /* a(1) = 0 */
        cache_size = 1;

        /* primes: start with 2 */
        primes_cap = 256;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_size = 1;

        initialized = true;
    }

    /* extend cache up to desired n */
    while (cache_size < n) {
        uint64_t p = next_candidate;

        /* trial division against known primes ≤ √p */
        bool is_prime = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t q = primes[i];
            unsigned __int128 q2 = (unsigned __int128)q * q;
            if (q2 > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store new prime */
            if (primes_size == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_size++] = p;

            /* update counts */
            if (p % 4 == 1) {
                ++count1;
                ++diff;
            } else if (p % 4 == 3) {
                ++count3;
                --diff;
            } /* p == 2 is already handled and not counted */

            /* record equality */
            if (diff == 0) {
                if (cache_size + 1 > cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[++cache_size] = count1; /* count1 == count3 at equality */
            }
        }

        /* advance to next odd candidate */
        next_candidate += 2;
    }

    return cache[n];
}
