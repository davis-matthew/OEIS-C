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
 * Helpers:   clump_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/clump_sum.h"

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
        seq_t *tmp = (seq_t*)realloc(clumps, new_cap * sizeof(seq_t));
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
