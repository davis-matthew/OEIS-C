/*
 * A227823  Numbers n with property that each sum of any k-subsequence of
 *          digits of n with k>=2 is prime.
 * https://oeis.org/A227823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) is the n-th positive integer such that every sum of any two
 *            or more of its decimal digits is a prime number
 * Range:     u8 n <= 38, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation performs a brute‑force search, memoizing all found
   terms so that calls made in any order remain fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Static cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity of cache */
    static seq_t next_candidate = 10; /* next integer to test (>=10) */

    /* Expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Test whether next_candidate satisfies the property */
        seq_t x = next_candidate;
        /* Extract decimal digits into an array (least significant first) */
        uint8_t dig[32];
        size_t d = 0;
        seq_t tmp = x;
        while (tmp > 0) {
            dig[d++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        if (d < 2) { /* need at least two digits */
            ++next_candidate;
            continue;
        }

        /* For numbers with more than 63 digits we give up: their
           superset enumeration would overflow the bitmask.  Such
           numbers are far beyond the range needed for any realistic
           n (e.g., the 10000th term is far smaller). */
        if (d > 63) {
            ++next_candidate;
            continue;
        }

        bool ok = true;
        /* Iterate over all non‑empty subsets using a bitmask.
           Only subsets of size >=2 need to be checked. */
        uint64_t max_mask = 1ULL << d;
        for (uint64_t mask = 1; mask < max_mask && ok; ++mask) {
            unsigned int bits = 0;
            unsigned int sum  = 0;
            for (size_t i = 0; i < d; ++i) {
                if (mask & (1ULL << i)) {
                    ++bits;
                    sum += dig[i];
                }
            }
            if (bits >= 2) {
                /* sum is small (<=9*d <= 9*63=567) */
                if (!is_prime_small(sum)) {
                    ok = false;
                }
            }
        }

        ++next_candidate; /* advance candidate for next loop iteration */

        if (!ok) {
            continue;    /* not a valid term */
        }

        /* Store valid term in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
#ifdef __cplusplus
            seq_t *new_mem = reinterpret_cast<seq_t *>(
                realloc(cache, new_cap * sizeof(seq_t)));
#else
            seq_t *new_mem = (seq_t *)
                realloc(cache, new_cap * sizeof(seq_t));
#endif
            if (!new_mem) {
                abort(); /* allocation failure */
            }
            cache    = new_mem;
            cache_cap = new_cap;
        }
        cache[cache_len++] = x;
    }

    return cache[(size_t)n - 1];
}
