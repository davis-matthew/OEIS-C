/*
 * A095994  Numbers k such that k - (largest digit of k) is prime.
 * https://oeis.org/A095994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A095994: numbers k such that k-(largest decimal digit of k) is prime.
   This implementation uses a simple trial‑division primality test and
   memoises previously found terms in a static cache.  All arithmetic is
   performed with the supplied seq_t type.  No external utilities are
   required. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Small helpers ---------------------------------------------------------- */

/* Return the integer square‑root of n (floor(sqrt(n))) using binary search. */
static seq_t floor_sqrt(seq_t n)
{
    if (n < 2)
        return n;

    seq_t low = 0, high = n, mid, square;
    while (low <= high) {
        mid = (low + high) >> 1;
        square = mid * mid;
        if (square <= n) {
            if (mid == UINT64_MAX)   /* avoid infinite loop for overflow */
                break;
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

/* Return the largest decimal digit in n. */
static seq_t largest_digit(seq_t n)
{
    if (n == 0)
        return 0;

    seq_t maxd = 0;
    while (n > 0) {
        seq_t d = n % 10;
        if (d > maxd)
            maxd = d;
        n = n / 10;
    }
    return maxd;
}

/* Deterministic trial‑division prime test that works for small numbers.
   For seq_t values that fit into idx_t this is fast enough for the
   sequence up to at least n = 10000. */
static bool is_prime_seq(seq_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if ((n & 1) == 0)
        return false;

    seq_t limit = floor_sqrt(n);
    for (idx_t i = 3; (seq_t)i <= limit; i += 2) {
        if (n % (seq_t)i == 0)
            return false;
    }
    return true;
}

/* Main sequence function --------------------------------------------------- */

/* Static cache of the sequence values.  The cache grows as needed. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t   cache_len = 0;   /* number of terms stored so far */
static idx_t   last_k    = 0;   /* last integer examined (1‑based) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset 1 – no valid term at n = 0 */

    /* Return from cache if the requested term is already known. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Initialise cache if it does not yet exist. */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Temporary variable for candidate integer k. */
    for (idx_t k_idx = last_k + 1; ; ++k_idx) {
        seq_t k = (seq_t)k_idx;
        seq_t maxd = largest_digit(k);
        seq_t diff  = k - maxd;

        if (diff >= (seq_t)2 && is_prime_seq(diff)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
            }
            cache[cache_len++] = k;
            if (cache_len >= n) {
                last_k = k_idx;
                return k;
            }
        }
    }
}
