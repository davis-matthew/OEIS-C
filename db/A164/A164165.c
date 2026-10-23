/*
 * A164165  Number of binary strings of length n with equal numbers of 0001
 *          and 1110 substrings.
 * https://oeis.org/A164165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts binary strings of length n that contain * the same
 *            number of 0001 substrings as 1110 substrings. * The counting is
 *            performed by a dynamic programme that * remembers the last three
 *            bits of the current string and * the current difference (#0001 –
 *            #1110). The DP runs * in O(n²) time (≈2·...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* maximum possible absolute difference is at most floor(n/4)
     * but we allocate an array that can hold differences from
     * -n to +n to keep indexing simple. */
    size_t offset   = (size_t)n;               /* position of diff = 0          */
    size_t diff_sz  = 2 * offset + 1;          /* number of different diff values */
    size_t row_sz   = diff_sz;                 /* one row          */
    size_t total_sz = 8 * row_sz;              /* 8 possible last-three-bit masks */

    /* Allocate arrays for current and next DP states.
     * calloc guarantees all entries start at 0. */
    seq_t *cur  = (seq_t*)calloc(total_sz, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(total_sz, sizeof(seq_t));
    if (!cur || !next) return 0;              /* allocation failure                 */

    /* start with an empty string: mask 0 (no last bits), diff 0 */
    cur[offset] = (seq_t)1;

    for (idx_t pos = 0; pos < n; ++pos) {
        /* For strings of current length 'pos', the reachable absolute
         * difference is at most floor(pos/4).  We will iterate only
         * over that range. */
        size_t diff_max_cur = pos / 4;
        size_t diff_start   = offset - diff_max_cur;
        size_t diff_end     = offset + diff_max_cur;

        /* Zero the next-state array for the indices that might be written
         * (others already zero from previous iteration). */
        for (size_t i = 0; i < total_sz; ++i) next[i] = (seq_t)0;

        /* Transition from current states to next states */
        for (int mask = 0; mask < 8; ++mask) {
            size_t base_cur  = mask * row_sz;
            size_t base_next = mask * row_sz;   /* placeholder, will change to new_mask */

            for (size_t di = diff_start; di <= diff_end; ++di) {
                seq_t val = cur[base_cur + di];
                if (!val) continue;            /* skip zero entries */

                int diff = (int)(di - offset); /* current difference */
                for (int b = 0; b <= 1; ++b) { /* append bit 0 or 1 */
                    int new_mask = ((mask << 1) | b) & 7;
                    int new_diff = diff;

                    if (pos >= 3) {            /* from now on a new 4‑bit pattern
                                                 * is formed */
                        int pat4 = (mask << 1) | b; /* 4 bits: last three + new bit */
                        if (pat4 == 0b0001) new_diff += 1;
                        else if (pat4 == 0b1110) new_diff -= 1;
                    }

                    size_t new_idx = (size_t)(new_diff + offset);
                    next[new_mask * row_sz + new_idx] += val;
                }
            }
        }

        /* Prepare for next iteration */
        seq_t *tmp   = cur;
        cur          = next;
        next         = tmp;
    }

    /* Sum counts of all masks at difference 0 */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < 8; ++mask) {
        result += cur[mask * row_sz + offset];
    }

    free(cur);
    free(next);
    return result;
}
