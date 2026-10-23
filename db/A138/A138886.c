/*
 * A138886  n-th run has length n-th nonprime number A018252, with digits 0
 *          and 1 only, starting with 0.
 * https://oeis.org/A138886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is produced by alternating runs of 0 and 1, where the length of the n‑th run
   is the n‑th nonprime number (starting with 1).  The sequence starts with 0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static bool initialized = false;
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;

static idx_t current_run = 1;          /* index of the current run */
static idx_t current_nonprime = 1;     /* length of the current run (the n‑th nonprime) */
static idx_t remaining_in_run = 1;     /* remaining positions in the current run */
static seq_t current_digit = 0;        /* 0 for first run, then toggles */

/* ensure that the cache contains values up to (but not including) index n+1 (1‑based) */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_size == 0) {
        /* initial capacity */
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_size < (size_t)n) {
        if (remaining_in_run == 0) {
            /* finish current run and prepare the next one */
            current_run++;
            /* find next nonprime number */
            current_nonprime++;
            while (is_prime(current_nonprime))
                current_nonprime++;
            remaining_in_run = current_nonprime;
            current_digit ^= 1; /* toggle between 0 and 1 */
        }
        /* grow the cache if necessary */
        if (cache_size == cache_capacity) {
            cache_capacity *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_size++] = current_digit;
        remaining_in_run--;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache = NULL;
        cache_size = 0;
        cache_capacity = 0;
        initialized = true;
    }
    if (n == 0) return 0; /* undefined for offset 0, but this is safe */
    ensure_cache_up_to(n);
    return cache[n - 1];
}
