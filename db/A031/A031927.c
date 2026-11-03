/*
 * A031927  Upper prime of a difference of 8 between consecutive primes.
 * https://oeis.org/A031927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 539, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 916 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A031927: upper prime of a difference of 8 between consecutive primes. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_prime = 0;
    static bool initialized = false;

    if (n < cache_len) {
        return cache[n];
    }

    if (!initialized) {
        last_prime = 1;                /* so that next_prime(last_prime+1) == 2 */
        initialized = true;
    }

    while (cache_len <= n) {
        uint64_t next_p = next_prime(last_prime + 1);
        uint64_t diff   = next_p - last_prime;
        if (diff == 8ULL) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_p;
        }
        last_prime = next_p;
    }
    return cache[n];
}
