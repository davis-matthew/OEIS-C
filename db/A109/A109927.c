/*
 * A109927  First primes p connected to two primes either by 2p+1 or 2p-1
 *          upward [downward (p-1)/2 or (p+1)/2].
 * https://oeis.org/A109927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p (>2, p ≠ 7) such that
   (p‑1)/2 or (p+1)/2 is prime, and 2p+1 or 2p‑1 is prime.
   The function builds the sequence by scanning successive primes
   and caching the results, so it can answer any index n ≥ 1 in
   O(n) time for the first n terms (subsequent calls reuse the cache). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static cache for computed terms */
static seq_t *cache = NULL;       /* dynamically allocated array of seq_t */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static size_t cache_size = 0;     /* number of terms stored in cache */

/* Last prime examined; used to resume scanning between calls */
static uint64_t last_examined = 2;   /* just before the first candidate (3) */

/* Append a term to the cache, expanding it if necessary */
static void push_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In the unlikely event realloc fails, the program will terminate,
           which is acceptable for this setting. */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe fallback */

    /* Extend the cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* Find the next prime greater than last_examined */
        uint64_t p = last_examined + 1;
        for (;;) {
            if (is_prime(p)) {
                last_examined = p;
                break;
            }
            ++p;
        }

        /* Skip excluded primes 2 and 7 */
        if (p == 2 || p == 7) continue;

        /* Upward check: 2p+1 or 2p‑1 is prime */
        bool up = false;
        uint64_t up1 = (uint64_t)((unsigned __int128)2 * p + 1);
        if (is_prime(up1)) up = true;
        uint64_t up2 = (uint64_t)((unsigned __int128)2 * p - 1);
        if (!up && is_prime(up2)) up = true;

        /* Downward check: (p‑1)/2 or (p+1)/2 is prime */
        bool down = false;
        uint64_t d1 = (p - 1) / 2;
        uint64_t d2 = (p + 1) / 2;
        if (d1 >= 2 && is_prime(d1)) down = true;
        if (!down && d2 >= 2 && is_prime(d2)) down = true;

        if (up && down) {
            push_cache((seq_t)p);
        }
    }

    /* cache is in ascending order, so index n‑1 gives the answer */
    return cache[(size_t)(n - 1)];
}
