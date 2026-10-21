/*
 * A097538  Subtract 2 from primes == 3 (mod 4).
 * https://oeis.org/A097538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097538: subtract 2 from primes ≡ 3 (mod 4).  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* Cached a(n) values */
static size_t cache_size = 0;        /* Number of cached entries */
static size_t cache_cap = 0;         /* Current capacity */
static idx_t next_candidate = 3;     /* Next odd number to test */

/* Return the n‑th term of the sequence, 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined for n == 0 */
        return 0;

    /* Fill the cache up to n. */
    while (cache_size < n) {
        idx_t p = next_candidate;

        /* Find the next prime ≡ 3 (mod 4). */
        while (!is_prime(p) || (p & 3) != 3)
            p += 2;            /* skip even numbers */

        /* Grow the cache if needed. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the term (prime minus 2). */
        cache[cache_size] = (seq_t)(p - 2);
        cache_size++;

        /* Next odd number after the prime. */
        next_candidate = p + 2;
    }

    return cache[n - 1];
}
