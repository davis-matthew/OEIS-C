/*
 * A085273  Difference between n-th composite number and its largest prime
 *          divisor.
 * https://oeis.org/A085273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085273: difference between the n-th composite number
   and its largest prime divisor.  This implementation enumerates
   composites in increasing order, caching the results so that
   subsequent calls for any n are answered in O(1) after the first
   evaluation.  The search uses the provided primality and
   largest‑prime‑divisor utilities. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_divisor.h"

static seq_t *cache = NULL;          /* 1‑based cache for a(n) */
static size_t cache_len = 0;          /* number of cached elements (last index) */
static idx_t composites_found = 0;    /* number of composites already processed */
static idx_t cur_num = 4;             /* next integer to test for compositeness */

static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len >= n) return;      /* already computed */

    /* Grow cache array.  +1 for 1‑based indexing. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return;   /* allocation failure – silently skip, caller will fail */
    cache = tmp;
    cache_len = new_len - 1;  /* last valid index now n */

    /* Enumerate composites until reaching index n. */
    while (composites_found < n) {
        if (!is_prime(cur_num)) {                 /* cur_num is composite */
            uint64_t gpf = largest_prime_divisor(cur_num);
            seq_t diff = (seq_t)cur_num - (seq_t)gpf;
            cache[composites_found + 1] = diff;   /* 1‑based */
            composites_found++;
        }
        cur_num++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n=0 – return zero for safety */
    ensure_cache(n);
    return cache[(size_t)n];
}
