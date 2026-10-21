/*
 * A212399  Number of binary arrays of length n+9 with no more than 5 ones in
 *          any length 10 subsequence (=50% duty cycle).
 * https://oeis.org/A212399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary arrays of length n+9 in which every
 *            consecutive block of 10 bits contains at most 5 ones
 * Range:     u8 none, u16 n <= 8, u32 n <= 26, u64 n <= 63, u128 n <= 136,
 *            big any n
 * Helpers:   grow_seq_array, pop9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/grow_seq_array.h"
#include "../utils/pop9.h"

static seq_t dp_cur[512];
static seq_t dp_next[512];
static seq_t *results = NULL;
static size_t results_size = 0;    /* number of slots currently allocated */
static idx_t curr_nmax = 0;        /* maximum n with a computed value */
static int  curr_L   = 0;          /* number of bits processed so far (0 at start) */
static int  init_done = 0;

/* Make sure the results array can hold the entry for n */
static void ensure_results_cap(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > results_size) {
        /* grow_seq_array updates results_size to the new size */
        grow_seq_array(&results, &results_size, need);
        /* initialise new slots to zero; safer for arithmetic types */
        for (size_t i = (size_t)results_size; i < need; ++i) results[i] = 0;
    }
}

/* Extend the DP until we have computed a(k) for all k <= target_n */
static void extend_to(idx_t target_n)
{
    int target_L = (int)(target_n + 9);          /* total length of the words */
    ensure_results_cap(target_n);

    if (!init_done) {
        memset(dp_cur, 0, sizeof(dp_cur));
        dp_cur[0] = 1;                           /* empty prefix */
        memset(dp_next, 0, sizeof(dp_next));
        init_done = 1;
    }

    while (curr_L < target_L) {
        int full_window = (curr_L >= 9);         /* a new window will have size 10 */
        memset(dp_next, 0, sizeof(dp_next));

        for (int mask = 0; mask < 512; ++mask) {
            seq_t cnt = dp_cur[mask];
            if (!cnt) continue;
            int ones = pop9(mask);

            /* append 0 */
            if (!full_window || ones <= 5) {
                int new_mask = (mask << 1) & 511; /* b = 0 */
                dp_next[new_mask] += cnt;
            }

            /* append 1 */
            if (!full_window || (ones + 1) <= 5) {
                int new_mask = ((mask << 1) & 511) | 1; /* b = 1 */
                dp_next[new_mask] += cnt;
            }
        }

        ++curr_L;                                /* we've added one more bit */

        /* store the total for this length if it corresponds to a valid n */
        if (curr_L >= 10) {
            idx_t n = (idx_t)(curr_L - 9);
            seq_t total = 0;
            for (int m = 0; m < 512; ++m) total += dp_next[m];
            results[n] = total;
        }

        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    curr_nmax = target_n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > curr_nmax) extend_to(n);
    return results[n];
}
