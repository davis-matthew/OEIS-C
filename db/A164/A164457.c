/*
 * A164457  Number of binary strings of length n with no substrings equal to
 *          0001, 0011, or 1010.
 * https://oeis.org/A164457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   counts binary strings of length n avoiding 0001, 0011, 1010
 *            Using a DP on the last three bits (8 states) with memoisation of
 *            already computed values
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 48, u64 n <= 97, u128 n <= 197,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sums = NULL;        /* cache of a(0..cur_len) */
static idx_t  cur_len  = 0;       /* highest computed index in sums */
static idx_t  cap      = 0;       /* current allocated size of sums */
static seq_t  dp[8]    = {0};     /* counts for current length (last 3 bits) */

seq_t a(idx_t n)
{
    /* Initialise cache and DP on first use */
    if (!sums) {
        sums = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!sums) return (seq_t)0;          /* allocation failed */
        sums[0] = (seq_t)1;                  /* a(0) = 1 (empty string) */
        cap   = 1;
        dp[0] = (seq_t)1;                    /* one valid empty suffix */
        cur_len = 0;
    }

    /* If we already have the value, return it directly */
    if (n <= cur_len) {
        return sums[n];
    }

    /* Grow cache if required */
    if (n + 1 > cap) {
        seq_t *new_sums = (seq_t *)realloc(sums, sizeof(seq_t) * (n + 1));
        if (!new_sums) return (seq_t)0;      /* allocation failed */
        sums = new_sums;
        cap   = n + 1;
    }

    /* Forbidden 4‑bit patterns: 0001, 0011, 1010 */
    const unsigned bad[3] = {1, 3, 10};

    /* Extend DP from cur_len up to n */
    for (idx_t i = cur_len; i < n; ++i) {
        seq_t next[8] = {0};
        int  check   = (i >= 3);              /* only check after 3 bits */

        for (int s = 0; s < 8; ++s) {
            if (dp[s] == (seq_t)0) continue;
            for (int b = 0; b < 2; ++b) {
                if (check) {
                    unsigned new4 = (unsigned)((s << 1) | b);
                    if (new4 == bad[0] || new4 == bad[1] || new4 == bad[2])
                        continue;
                }
                unsigned ns = (unsigned)(((s << 1) | b) & 7);
                next[ns] += dp[s];
            }
        }

        /* Move to next length */
        for (int s = 0; s < 8; ++s) dp[s] = next[s];

        /* Sum up all valid states for this new length */
        seq_t total = (seq_t)0;
        for (int s = 0; s < 8; ++s) total += dp[s];
        sums[i + 1] = total;
    }

    cur_len = n;
    return sums[n];
}
