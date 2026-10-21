/*
 * A063167  Composite numbers which in base 8 contain their largest proper
 *          factor as a substring.
 * https://oeis.org/A063167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   contains_octal, to_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063167: Composite numbers that in base‑8 contain their largest
 * proper factor as a substring.
 * The function searches forward until the n‑th term is found.
 * It memoizes found terms, so subsequent calls are fast.
 * No external libraries beyond the C standard are required. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/contains_octal.h"
#include "../utils/to_octal.h"

/* Smallest prime factor of a 64‑bit unsigned integer.
 * Uses simple trial division (fast enough for the required range). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 3) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if ((n % p) == 0) return p;
    }
    return n;          /* n is prime */
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    static seq_t *cache       = NULL; /* list of found terms   */
    static size_t cache_len   = 0;    /* how many terms stored */
    static size_t cache_cap   = 0;    /* allocated capacity    */
    static uint64_t last_done = 3;    /* last checked number   */

    if (n == 0) return 0;            /* undefined for n<1 */

    /* Return a cached value if already available. */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* Generate terms until we reach the n‑th. */
    while (cache_len < (size_t)n) {
        ++last_done;                       /* next candidate */
        uint64_t spf = smallest_prime_factor(last_done);
        if (spf == last_done) continue;    /* prime, skip */

        uint64_t largest = last_done / spf;    /* largest proper factor */
        if (!contains_octal(last_done, largest)) continue;

        /* ensure cache capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) abort();        /* fatal */
            if (cache) memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)last_done;
    }

    return cache[n - 1];
}
