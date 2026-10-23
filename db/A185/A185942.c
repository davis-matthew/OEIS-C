/*
 * A185942  First of a run of 4 or more consecutive primes which all equal 1
 *          (mod 3).
 * https://oeis.org/A185942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 141, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A185942: record the first prime of every run of
   4 or more consecutive primes congruent to 1 (mod 3).  Runs may overlap,
   so each prime that begins such a run is output.  The function
   generates primes on demand via next_prime() and maintains a sliding
   window of the last four qualifying primes. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"   /* provides uint64_t next_prime(uint64_t) */

/* Cache of computed terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Sliding window of the last up to four primes ≡ 1 (mod 3). */
static uint64_t run_buf[4] = {0};
static int      run_head  = 0;   /* index of oldest element in run_buf */
static int      run_size  = 0;   /* number of elements currently stored */

/* Append a value to the cache, resizing as needed. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Get the next prime after the last one we examined. */
        static uint64_t last_prime = 2;
        last_prime = next_prime(last_prime);

        /* If the prime satisfies the congruence, update the sliding window. */
        if (last_prime % 3ULL == 1ULL) {
            /* If we already have four elements, discard the oldest first. */
            if (run_size == 4) {
                run_head = (run_head + 1) % 4;
                run_size = 3;
            }
            /* Insert the new prime at the back. */
            int pos = (run_head + run_size) % 4;
            run_buf[pos] = last_prime;
            run_size++;

            /* When we have a full window, record the start of the run. */
            if (run_size == 4) {
                cache_append((seq_t)run_buf[run_head]);
            }
        } else {
            /* Any prime not ≡ 1 (mod 3) breaks the run. Clear the window. */
            run_head = 0;
            run_size = 0;
        }
    }

    return cache[n - 1];
}
