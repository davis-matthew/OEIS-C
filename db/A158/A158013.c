/*
 * A158013  100*a(n)+13 and 100*a(n)+27 are consecutive primes, i.e., a prime
 *          gap 14.
 * https://oeis.org/A158013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 18, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A158013: a(n) is the n‑th integer a such that
 * 100*a+13 and 100*a+27 are consecutive primes (prime gap 14). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;
static idx_t cache_size  = 0;
static idx_t cap         = 0;
static idx_t last_checked = 0;        /* last a that was examined */

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Initialise or grow the cache. */
    if (cap == 0) {
        cap   = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Search for new terms until we obtain the nth. */
    for (;;) {
        /* Candidate a. */
        last_checked++;
        seq_t a_candidate = last_checked;

        /* Endpoints of the 14‑digit range. */
        seq_t p1 = a_candidate * (seq_t)100 + (seq_t)13;
        seq_t p2 = p1 + (seq_t)14;          /* 100*a+27 */

        /* Both must be prime. */
        if (!is_prime_big(p1) || !is_prime_big(p2))
            continue;

        /* Check for primes strictly between p1 and p2.
         * The only odd offsets in (13,27) are 4,6,8,10,12. */
        if (is_prime_big(p1 + (seq_t)4) ||
            is_prime_big(p1 + (seq_t)6) ||
            is_prime_big(p1 + (seq_t)8) ||
            is_prime_big(p1 + (seq_t)10)||
            is_prime_big(p1 + (seq_t)12))
            continue;

        /* Valid a – store it. */
        if (cache_size == cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }
        cache[cache_size++] = a_candidate;

        /* If we have reached the requested index, return it. */
        if (cache_size == n)
            return cache[n - 1];
    }
}
