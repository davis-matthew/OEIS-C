/*
 * A117404  Triangular numbers for which the sum of the digits is a square.
 * https://oeis.org/A117404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 7821, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9984 (2026-11-03)
 * Helpers:   digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force: generate triangular numbers T_k = k*(k+1)/2 and keep those
   whose decimal digit sum is a perfect square.  The sequence a(n) is the
   n‑th such triangular number (n is 1‑based). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    /* Dynamically maintain a cache of the found terms. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t length   = 0;

    /* Current state of the triangular number generator. */
    static idx_t  next_k = 0;      /* k such that next_T = T_k */
    static seq_t next_T = 0;       /* current triangular number T_k */

    /* Return cached value if already computed. */
    if (n <= length) return cache[n - 1];

    /* Generate triangular numbers until we have at least n terms. */
    while (length < n) {
        /* Check whether the digit sum of next_T is a perfect square. */
        seq_t sum  = digit_sum(next_T);
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            /* Ensure sufficient capacity and store the term. */
            if (length == capacity)
                grow_seq(&cache, &capacity, length + 1);
            cache[length++] = next_T;
        }
        /* Advance to the next triangular number: T_{k+1} = T_k + (k+1). */
        next_k++;
        next_T += next_k;
    }

    return cache[n - 1];
}
