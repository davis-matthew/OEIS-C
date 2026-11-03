/*
 * A100278  Group the terms of A001511 into clumps so that the sum in each
 *          clump is just larger than the sum of the previous clump; sequence
 *          gives the sums of the clumps.
 * https://oeis.org/A100278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the sum of the n-th clump when grouping the * sequence
 *            A001511 (a(n)=1+v2(n)) into successive clumps so that each *
 *            clump sum is just larger than the previous clump sum
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute a single clump sum. */
static inline seq_t clump_sum(idx_t start_index, seq_t prev_sum,
                               idx_t *current_index)
{
    seq_t sum = 0;
    while (sum <= prev_sum) {
        /* value of A001511 at position 'current_index' */
        idx_t v = *current_index;
        seq_t term = (seq_t)1 + (seq_t)__builtin_ctzll((unsigned long long)v);
        sum += term;
        (*current_index)++;
    }
    return sum;
}

/* Memoized storage of already computed clump sums. */
static seq_t *clumps = NULL;
static size_t clumps_cap = 0;
static size_t clumps_len = 0;           /* number of computed clumps   */
static idx_t  term_index = 1;           /* next index of A001511 term  */
static seq_t  prev_sum  = 0;            /* sum of the previous clump   */

static void ensure_capacity(size_t needed)
{
    if (needed > clumps_cap) {
        size_t new_cap = clumps_cap ? clumps_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, clumps, new_cap);
        if (!tmp) abort();
        clumps = tmp;
        clumps_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based; the caller guarantees n>=1. */
    if (n == 0)
        return 0;   /* undefined behaviour, but avoid crash */

    if (clumps_len < (size_t)n) {
        /* compute missing clumps up to 'n' */
        ensure_capacity((size_t)n);
        while (clumps_len < (size_t)n) {
            seq_t sum = clump_sum(term_index, prev_sum, &term_index);
            prev_sum = sum;
            clumps[clumps_len++] = sum;
        }
    }
    return clumps[n - 1];
}
