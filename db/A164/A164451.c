/*
 * A164451  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 1100.
 * https://oeis.org/A164451
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of binary strings of length n with no
 * substrings 0001, 0010, or 1100.
 * Implementation: A simple dynamic programming over the last
 * three bits. State = 3‑bit suffix of the current string.
 * dp[len][s] = number of valid strings of length len ending
 * with suffix s. Transition: append 0 or 1, reject if the new 4‑bit
 * window equals 0001 (1), 0010 (2), or 1100 (12).  The DP table
 * is stored in a flat array dp[len*8 + s] and built incrementally
 * so repeated calls for arbitrary n are O(n) overall.
 */

#include <stdlib.h>

static seq_t *dp = NULL;    /* dp[len*8 + state] */
static idx_t max_n = 0;     /* highest n for which dp is built */

seq_t a(idx_t n)
{
    /* Undefined for n < 4; return 0 as a neuter value. */
    if (n < 4) return 0;

    /* First call: initialise dp with row 0 only. */
    if (dp == NULL) {
        dp = (seq_t *)calloc( (size_t)1 * 8 , sizeof(seq_t));
        if (dp == NULL) abort();   /* allocation failure */
        dp[0] = 1;                 /* dp[0][0] = 1 */
        max_n = 0;
    }

    /* Grow dp to support new n if necessary. */
    if (n > max_n) {
        idx_t new_max = n;
        size_t new_size = (size_t)(new_max + 1) * 8 * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(dp, new_size);
        if (tmp == NULL) abort();
        dp = tmp;

        /* Zero newly allocated rows (from max_n+1 up to new_max). */
        size_t start = (size_t)(max_n + 1) * 8;
        size_t end   = (size_t)(new_max + 1) * 8;
        for (size_t i = start; i < end; ++i)
            dp[i] = 0;

        /* Build DP rows from max_n up to n-1. */
        for (idx_t len = max_n; len < n; ++len) {
            for (int s = 0; s < 8; ++s) {
                seq_t val = dp[(size_t)(len * 8 + s)];
                if (val == 0) continue;
                for (int bit = 0; bit < 2; ++bit) {
                    int new_state = ((s << 1) | bit) & 7;
                    if (len >= 3) {
                        int pattern = ((s << 1) | bit);
                        if (pattern == 1 || pattern == 2 || pattern == 12)
                            continue; /* forbidden */
                    }
                    dp[(size_t)((len + 1) * 8 + new_state)] += val;
                }
            }
        }
        max_n = n;
    }

    /* Sum over all ending states to obtain a(n). */
    seq_t sum = 0;
    for (int s = 0; s < 8; ++s)
        sum += dp[(size_t)(n * 8 + s)];
    return sum;
}
