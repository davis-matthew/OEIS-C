/*
 * A087679  Numbers k such that both k+2 and k-2 are prime.
 * https://oeis.org/A087679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 679, u32 n <= 830, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087679 – Numbers k such that both k‑2 and k‑+2 are prime.
   The function scans integers, tests primality, and caches the results.
   It works for any build that supplies the helper
   `is_prime_big` and `grow_seq_array`. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;      /* dynamic array of found terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored so far */
static seq_t next_k = (seq_t)5;  /* next candidate integer to test */

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, guard against bad input */

    /* Ensure the cache can store at least n terms. */
    if (cache_len < (size_t)n && cache_cap < (size_t)n)
        grow_seq_array(&cache, &cache_cap, (size_t)n);

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        /* Test the cousin‑prime condition for the current candidate. */
        if (is_prime_big(next_k - (seq_t)2) && is_prime_big(next_k + (seq_t)2))
        {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap)
                grow_seq_array(&cache, &cache_cap, cache_cap * 2 + 1);

            cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
