/*
 * A145387  Natural numbers n such that n-1 and n+1 are prime, also (sum of
 *          digits) + 1 and (sum of digits) - 1 is prime.
 * https://oeis.org/A145387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 343, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 920 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: n such that n‑1 and n+1 are prime and digit_sum(n)±1 are prime.
   This implementation caches the found terms and extends the cache on
   demand using a simple scan over even numbers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;   /* array of found terms */
    static size_t cache_size = 0;      /* number of terms stored   */
    static size_t cap        = 0;      /* memory capacity of cache */
    static idx_t next_candidate = 4;   /* next even number to test */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cap   = 16;
        cache = seq_alloc_array(cap);   /* allocate array of size 'cap'  */
    }

    /* extend the cache until we have at least 'n' terms */
    while (cache_size < n) {
        /* test the current candidate */
        if (is_prime(next_candidate - 1) && is_prime(next_candidate + 1)) {
            /* compute sum of decimal digits of next_candidate */
            idx_t t = next_candidate;
            idx_t digit_sum = 0;
            while (t > 0) {
                digit_sum += t % 10;
                t /= 10;
            }
            if (digit_sum >= 1 && is_prime(digit_sum - 1) && is_prime(digit_sum + 1)) {
                /* cast to seq_t for storage (it fits, as seq_t is an unsigned integer) */
                append_candidate(&cache, &cache_size, &cap, (seq_t)next_candidate);
            }
        }
        next_candidate += 2;   /* only even numbers can satisfy the condition */
    }

    /* return the requested element (1‑based index) */
    return cache[(size_t)(n - 1)];
}
