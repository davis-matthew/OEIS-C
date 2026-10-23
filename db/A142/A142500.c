/*
 * A142500  Primes congruent to 40 mod 51.
 * https://oeis.org/A142500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 204, u32 n <= 113, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 40 (mod 51)
   a(n) = the nth prime p with p % 51 == 40  (offset 1) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* The sequence is stored in a cache that grows on demand.
   We keep the last integer tested for primality so we don't
   examine the same numbers twice even if calls come in
   arbitrary order. */
static seq_t *cache = NULL;           /* Array of sequence values   */
static size_t cache_size = 0;         /* Number of entries stored  */
static size_t cache_cap   = 0;         /* Current allocation size   */
static uint64_t last_checked = 1;     /* Last number examined      */

/* Helper to grow a buffer to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t new_size = new_cap + (new_cap >> 1) + 16;  /* 1.5× + 16 */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_cap = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    /* Build cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        last_checked++;
        if (last_checked % 51 == 40) {
            seq_t cand = (seq_t)last_checked;
            if (is_prime_big(cand)) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = cand;
            }
        }
    }
    return cache[(size_t)n - 1];
}
