/*
 * A185587  Irregular triangle read by rows: row n gives a list of the lengths
 *          of the free spaces at the n-th stage in a Rule 18 cellular
 *          automaton.
 * https://oeis.org/A185587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth free space (gap) between consecutive 1s in
 *            successive generations of a Rule 18 cellular automaton started
 *            from a single 1. The program simulates the automaton
 *            generation‑by‑generation, records the gaps, and memoises them
 * Range:     u8 n <= 6305, u16 n <= 9920, u32 n <= 9920, u64 n <= 9920,
 *            u128 n <= 9920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* memoised sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* current CA state (old generation) */
    static uint8_t *old = NULL;
    static size_t old_len = 0;
    static bool initialized = false;

    /* initialise first generation only once */
    if (!initialized) {
        old_len = 1;
        old = (uint8_t *)malloc(old_len);
        if (!old) exit(EXIT_FAILURE);
        old[0] = 1;               /* single 1 at generation 0 */
        initialized = true;
    }

    /* generate more terms until we have n of them */
    while (cache_len < (size_t)n) {
        /* new generation has width old_len + 2 */
        size_t new_len = old_len + 2;
        uint8_t *new_arr = (uint8_t *)malloc(new_len);
        if (!new_arr) exit(EXIT_FAILURE);

        /* compute new state: new[i] = old[i-2] XOR old[i]   */
        for (size_t i = 0; i < new_len; ++i) {
            int left  = 0;
            int right = 0;
            if (i >= 2 && (i - 2) < old_len) left  = old[i - 2];
            if (i < old_len)                     right = old[i];
            new_arr[i] = (uint8_t)(left ^ right);
        }

        /* record gaps between consecutive 1s in new_arr */
        int prev = -1;
        for (size_t i = 0; i < new_len; ++i) {
            if (new_arr[i]) {
                if (prev != -1) {
                    idx_t gap = (idx_t)(i - prev - 1);
                    grow_seq(&cache, &cache_cap, cache_len + 1);
                    cache[cache_len++] = (seq_t)gap;
                }
                prev = (int)i;
            }
        }

        /* prepare for next generation */
        free(old);
        old     = new_arr;
        old_len = new_len;
    }

    return cache[(size_t)n - 1];
}
