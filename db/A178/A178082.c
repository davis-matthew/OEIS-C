/*
 * A178082  Numbers k for which 5*k-4, 5*k-2, 5*k+2, and 5*k+4 are primes.
 * https://oeis.org/A178082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k such that 5*k-4, 5*k-2, 5*k+2, 5*k+4
 *            are all prime
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n) {
    /* Static cache for results across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;          /* next candidate to test */

    if (n == 0) {
        return 0;   /* undefined for n<1, but safe */
    }

    /* Keep generating until we have at least n elements */
    while (cache_len < (size_t)n) {
        /* Test candidate k = next_k */
        idx_t p = 5 * next_k - 4; /* base prime in the quadruplet */
        if (is_prime(p) &&
            is_prime(p + 2) &&
            is_prime(p + 6) &&
            is_prime(p + 8)) {
            /* Store the valid k */
            if (cache_len + 1 > cache_cap) {
                /* Ensure capacity for one more element */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len] = (seq_t)next_k;
            cache_len++;
        }
        next_k++;
    }

    return cache[n - 1];
}
