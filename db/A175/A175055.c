/*
 * A175055  a(n) = decimal equivalent of {A033015(n) written in binary, and
 *          each run of 0's reduced in length by one digit, and each run of
 *          1's reduced in length by one digit}.
 * https://oeis.org/A175055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal equivalent of {A033015(n) written in binary, and
 *            each run of 0's reduced in length by one digit, and each run of
 *            1's reduced in length by one digit}
 * Range:     u8 n <= 87, u16 n <= 4179, u32 n <= 8757, u64 n <= 8757,
 *            u128 n <= 8757, big any n
 * Helpers:   compute_a_from_m, has_run_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_a_from_m.h"
#include "../utils/has_run_single.h"

/* Static cache for computed a(n) values.  All indices are 1‑based. */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap   = 0;
static size_t a_cache_len   = 0;   /* number of stored values */
static uint64_t next_m      = 1;   /* next integer to test for A033015 */

/* Ensure cache has capacity for at least 'needed' elements. */
static void ensure_a_cache_capacity(size_t needed)
{
    if (needed <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache_cap = new_cap;
}

/* Main function requested by the problem.  Computes a(n).        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset, behaviour unrestricted */

    /* extend cache up to n */
    while (a_cache_len < (size_t)n) {
        /* find next integer whose binary representation has no run of length 1 */
        while (has_run_single(next_m)) {
            ++next_m;
        }
        /* compute a value from this integer */
        seq_t val = compute_a_from_m(next_m);

        /* store */
        ensure_a_cache_capacity(a_cache_len + 1);
        a_cache[a_cache_len++] = val;

        ++next_m;
    }
    return a_cache[n - 1];   /* cache is 0‑based */
}
