/*
 * A155945  Numbers n such that 24*n + 13 is not prime.
 * https://oeis.org/A155945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 936 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers n such that 24*n + 13 is not prime.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored */
    static idx_t last_checked = 0;   /* last candidate examined */

    if (n == 0) return 0;       /* offset is 1, but guard anyway */

    /* Allocate cache on first use */
    if (!cache) {
        cache = mem_new(seq_t, 1);
        if (!cache) return 0;
        cap = 1;
    }

    /* Produce terms until we have at least n of them */
    while (len < n) {
        idx_t candidate = last_checked + 1;

        /* Search for the next numbers that satisfy the condition */
        while (true) {
            idx_t test_val = 24 * candidate + 13;

            if (!is_prime(test_val)) {
                if (cap <= len)          /* grow array if needed */
                    grow_seq(&cache, &cap, len + 1);
                cache[len++] = (seq_t)candidate;
            }

            if (len >= n) break;         /* enough terms obtained */
            ++candidate;
        }

        last_checked = candidate;       /* next search start point */
    }

    return cache[n - 1];
}
