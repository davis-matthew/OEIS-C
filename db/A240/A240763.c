/*
 * A240763  Preferential arrangements of 1, 2, 3, ... things in one-line
 *          notation, arranged lexicographically.
 * https://oeis.org/A240763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 633, u32 n <= 8252, u64 n <= 8252,
 *            u128 n <= 8252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: The sequence lists all preferential arrangements (ordered
 * partitions) in one-line notation for all sizes.  Each arrangement is
 * encoded as a decimal integer whose digits are the block numbers
 * (starting at 1).  The entries are ordered lexicographically
 * (which, within a fixed length, is the same as increasing numeric
 * value).  The function returns the nth element of this ordered
 * list (1‑based index).
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_L   9                  /* maximum number of digits that may
                                     * appear in a block number (0‑9)   */
#define ALL_MASK (1U << MAX_L)     /* mask for all bits up to MAX_L     */
#define UNKNOWN ((idx_t)(-1))        /* sentinel for uncomputed DP cells */

/* DP table: dp[L][mask][rem] – number of completions of length 'rem'
 * that, when appended to a prefix using digits 1..L whose used digits
 * are described by 'mask', result in a contiguous set {1..k}.           */
static idx_t dp[10][ALL_MASK][10] = { UNKNOWN };

/* counts of valid strings of length L */
static idx_t count_len[10] = { 0 };

/* cumulative sums: cum[L] = total strings of lengths 1..L */
static idx_t cum[10]  = { 0 };
static bool dp_init = false;
static bool cum_init = false;

/* Recursive DP: number of valid strings of length 'rem' that can be
 * appended to a prefix having used bits 'mask'.  Digits allowed are
 * 1..L.  The final set of used digits must be contiguous from 1.   */
static idx_t count_complete(int L, unsigned mask, int rem) {
    if (rem == 0) {
        /* finished: check if used digits form {1..k} */
        return (mask & (mask + 1)) == 0 ? 1 : 0;
    }
    idx_t *cell = &dp[L][mask][rem];
    if (*cell != UNKNOWN) return *cell;
    idx_t total = 0;
    for (int d = 1; d <= L; ++d) {
        unsigned newmask = mask | (1U << (d - 1));
        total += count_complete(L, newmask, rem - 1);
    }
    *cell = total;
    return total;
}

/* Ensure DP tables for length L are initialized.  Also fill count_len[L]
 * with the total number of valid strings of that length.                       */
static void ensure_dp(int L) {
    for (unsigned mask = 0; mask < ALL_MASK; ++mask) {
        for (int rem = 0; rem <= L; ++rem) {
            dp[L][mask][rem] = UNKNOWN;
        }
    }
    count_len[L] = count_complete(L, 0, L);
}

/*--------------------------------------------------------------------------*/
/*  seq_t a(idx_t n)  -  returns the nth term of the sequence                */
seq_t a(idx_t n) {
    /* initialise DP and cumulative tables once */
    if (!dp_init) {
        for (int L = 1; L <= MAX_L; ++L) {
            ensure_dp(L);
        }
        dp_init = true;
    }

    if (!cum_init) {
        cum[0] = 0;
        for (int L = 1; L <= MAX_L; ++L) {
            cum[L] = cum[L - 1] + count_len[L];
        }
        cum_init = true;
    }

    /* find length L such that cum[L-1] < n <= cum[L] */
    int L = 1;
    while (L <= MAX_L && cum[L] < n) ++L;
    if (L > MAX_L) return 0;   /* should not happen for the supported size */

    /* zero‑based index inside strings of length L */
    idx_t r = n - cum[L - 1] - 1;

    /* construct the r-th lexicographic string of length L */
    seq_t value = 0;
    unsigned mask = 0;
    int rem = L;
    for (int pos = 0; pos < L; ++pos) {
        for (int d = 1; d <= L; ++d) {
            unsigned newmask = mask | (1U << (d - 1));
            idx_t cnt = count_complete(L, newmask, rem - 1);
            if (r < cnt) {
                /* choose this digit */
                value = value * (seq_t)10 + (seq_t)d;
                mask = newmask;
                rem--;
                break;
            } else {
                r -= cnt;   /* skip block of strings starting with this digit */
            }
        }
    }
    return value;
}
