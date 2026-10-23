/*
 * A154486  a(n+1)-+a(n)=prime, a(1)=0,a(2)=5.
 * https://oeis.org/A154486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A154486
    a(1)=0, a(2)=5.
    Recurrence:  a(n+1) = smallest integer x > a(n) such that
                 a(n)+x and |x-a(n)| are both prime.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static cache holding the sequence values a(1), a(2), …   */
static seq_t *cache   = NULL;   /* array of terms */
static size_t cache_len = 0;    /* number of elements stored   */
static size_t cache_cap = 0;    /* allocated array size        */

/* Initialise the cache with the first two terms. */
static void init_cache(void)
{
    if (cache != NULL) return;   /* already initialised */

    cache_cap = 8;                /* initial capacity       */
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)           /* allocation failure */
        return;

    cache[0] = (seq_t)0;         /* a(1) */
    cache[1] = (seq_t)5;         /* a(2) */
    cache_len = 2;
}

/* Grow the cache until it contains a(n) (1‑based index). */
static void ensure_filled(idx_t n)
{
    if (cache == NULL) init_cache();
    if (n <= 1) return;          /* a(1) already present */

    /* Target zero‑based index for a(n). */
    size_t target = (size_t)n - 1;

    /* Grow the cache one element at a time. */
    while (cache_len <= target) {
        /* Ensure capacity. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return;   /* allocation failure – nothing we can do */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the next term. */
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + 1;      /* first candidate > prev */

        /* Find the minimal candidate such that both
           sum = prev + cand  and  diff = cand - prev
           are prime numbers. */
        while (true) {
            seq_t sum = prev + cand;
            seq_t diff = cand - prev;   /* >= 1 because cand > prev */

            if (is_prime_big(sum) && is_prime_big(diff))
                break;

            cand++;
        }

        cache[cache_len] = cand;
        cache_len++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* n < 1 is undefined – return 0 safely */
    ensure_filled(n);
    return cache[(size_t)n - 1];
}
