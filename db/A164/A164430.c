/*
 * A164430  Number of binary strings of length n with no substrings equal to
 *          0000 0011 or 1100.
 * https://oeis.org/A164430
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no substrings
 *            equal to 0000, 0011 or 1100, offset 4 (first term a(4)=13). The
 *            computation uses a DFA with 8 states (last 3 bits). An iterative
 *            DP fills the sequence up to the largest requested n
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 40, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The function signature requested by the build system */
seq_t a(idx_t n)
{
    /* Internal static state for memoisation */
    static seq_t *seq = NULL;        /* cached sequence values */
    static size_t seq_cap = 0;       /* allocated size of seq */
    static idx_t  max_n_computed = 0;/* last n for which seq[n] is valid */
    static seq_t  dp_cur[8];         /* state distribution for current length */

    /* Helper: forbid patterns 0000 (0), 0011 (3), 1100 (12) */
    static const bool forbidden[16] = {
        /* 0-15 in binary: 0000 0001 0010 0011 0100 0101 0110 0111
           1000 1001 1010 1011 1100 1101 1110 1111 */
        true,  false, false, true,  false, false, false, false,
        false, false, false, false, true, false, false, false
    };

    /* Return 0 for n < 4 (offset not defined).  The sequence is only
       defined for n >= 4, but the caller may request smaller values. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* Initialise static data on first call or when capacity needs expansion. */
    if (seq == NULL || seq_cap <= n) {
        /* Allocate new array with some extra space to reduce reallocs. */
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation error; return 0 as safe fallback */
        seq = tmp;
        seq_cap = new_cap;

        /* Zero out the newly allocated region that we might need. */
        for (size_t i = max_n_computed + 1; i <= n; ++i) {
            seq[i] = (seq_t)0;
        }
    }

    /* If we already have the desired value computed, return it immediately. */
    if (n <= max_n_computed) {
        return seq[n];
    }

    /* Set up initial state distribution for length 3 */
    if (max_n_computed == 0) {
        for (int s = 0; s < 8; ++s) {
            dp_cur[s] = (seq_t)1; /* each 3‑bit suffix occurs once */
        }
        seq[0] = seq[1] = seq[2] = (seq_t)0; /* values below offset not used */
        seq[3] = (seq_t)0; /* placeholder */
        max_n_computed = 3;
    }

    /* Iterate from the current length+1 up to n, computing DP transitions. */
    for (idx_t len = max_n_computed + 1; len <= n; ++len) {
        seq_t dp_next[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        for (int s = 0; s < 8; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            /* Add bit 0 */
            if (!forbidden[(s << 1) | 0]) {
                int ns = ((s << 1) & 7);
                dp_next[ns] += val;
            }
            /* Add bit 1 */
            if (!forbidden[(s << 1) | 1]) {
                int ns = ((s << 1) & 7) | 1;
                dp_next[ns] += val;
            }
        }

        /* Sum all states to obtain the total count for this length */
        seq_t total = 0;
        for (int i = 0; i < 8; ++i) {
            total += dp_next[i];
        }
        seq[len] = total;
        /* Prepare for next iteration */
        for (int i = 0; i < 8; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }

    max_n_computed = n;
    return seq[n];
}
