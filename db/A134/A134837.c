/*
 * A134837  Composite numbers where the sum of the smallest prime factor and
 *          the largest prime factor is an odd composite.
 * https://oeis.org/A134837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   _smallest_prime_factor, isPrimeTrial, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th composite number whose smallest prime factor
 * plus its largest prime factor is an odd composite. */

/* Use available library functions. */
/* If any of these fails to compile, the header names may need
 * adjustment to match the build. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/largest_prime_factor.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */

/* Append a new term to the cache, expanding storage if necessary. */
static void cache_append(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return the n-th term (1‑based).  For n==0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0 */

    /* start searching from the smallest composite (4) */
    static uint64_t candidate = 4;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next qualifying composite number */
        for (;;) {
            /* Skip odd numbers – they cannot satisfy spf=2 */
            if (candidate & 1ULL) {
                ++candidate;
                continue;
            }

            /* Skip powers of two: no odd prime factor, lpf=0 */
            if ((candidate & (candidate - 1ULL)) == 0ULL) {
                ++candidate;
                continue;
            }

            /* Skip primes (though even primes are impossible) */
            if (isPrimeTrial((seq_t)candidate)) {
                ++candidate;
                continue;
            }

            /* Obtain smallest and largest prime factors (uint64_t) */
            uint64_t spf = _smallest_prime_factor(candidate);
            uint64_t lpf = largest_prime_factor(candidate);
            if (lpf == 0) {            /* shouldn't happen for composite > 2 */
                ++candidate;
                continue;
            }

            seq_t sum = (seq_t)spf + (seq_t)lpf;

            /* sum must be odd */
            if ((sum & 1ULL) == 0) {
                ++candidate;
                continue;
            }

            /* sum must be composite */
            if (isPrimeTrial(sum)) {
                ++candidate;
                continue;
            }

            /* Found a valid term */
            break;
        }

        /* Store the term and advance to the next candidate */
        cache_append((seq_t)candidate);
        ++candidate;
    }

    /* Return the required term (n is 1‑based) */
    return cache[n - 1];
}
