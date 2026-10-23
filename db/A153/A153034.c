/*
 * A153034  Numbers with adjacent 0's and no adjacent 1's in binary expansion.
 * https://oeis.org/A153034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with an adjacent pair of 0’s and no adjacent pair of 1’s in binary expansion
 * Computes a(n) by constructing the n‑th binary string that
 *  • starts with 1,
 *  • has no “11” as a substring,
 *  • contains at least one “00”.
 * No hard‑coded table; uses a small DP that is computed once.
 */
#include <stdint.h>
#include <stdbool.h>

#define MAX_LEN 128          /* enough for n up to many thousands */

seq_t a(idx_t n)
{
    /* DP table: dp[rem][prev][has00] = number of ways to fill `rem` bits
       starting from state (prev, has00) such that the final string has at
       least one adjacent pair of 0’s.  Count values fit in 64 bits for
       the required range of `n`. */
    static uint64_t dp[MAX_LEN + 1][2][2];
    static uint64_t cumulative[MAX_LEN + 1];   /* cumulative count up to each length */
    static bool ready = false;

    if (!ready) {
        /* Base case: no bits left to place – valid only if has00 already true */
        for (idx_t prev = 0; prev < 2; ++prev)
            for (idx_t h = 0; h < 2; ++h)
                dp[0][prev][h] = h ? 1 : 0;

        /* Bottom‑up DP for all remaining lengths */
        for (idx_t rem = 1; rem <= MAX_LEN; ++rem) {
            for (idx_t prev = 0; prev < 2; ++prev) {
                for (idx_t h = 0; h < 2; ++h) {
                    uint64_t total = 0;

                    /* Place a 0.  A new pair of zeros is created if prev==0. */
                    uint64_t new_h = h | (prev == 0);
                    total += dp[rem - 1][0][new_h];

                    /* Place a 1, only allowed if prev==0. */
                    if (prev == 0)
                        total += dp[rem - 1][1][h];

                    dp[rem][prev][h] = total;
                }
            }
        }

        /* Cumulative counts of admissible strings of exact length ℓ. */
        cumulative[0] = 0;
        for (idx_t len = 1; len <= MAX_LEN; ++len) {
            /* Start with leading 1, remaining len-1 bits, no adjacent 1 at start. */
            cumulative[len] = cumulative[len - 1] + dp[len - 1][1][0];
        }

        ready = true;
    }

    /* Find the binary length of the n‑th number. */
    idx_t len = 1;
    while (len <= MAX_LEN && cumulative[len] < n)
        ++len;
    if (len > MAX_LEN)   /* should not happen for the specified limits */
        return 0;

    /* Index within the group of length `len`. */
    idx_t within = n - cumulative[len - 1] - 1;

    /* Construct the binary representation bit by bit. */
    seq_t value = 1;            /* first bit is always 1 */
    idx_t remaining = len - 1;  /* bits still to place */
    idx_t prev = 1;             /* last bit placed */
    idx_t has00 = 0;            /* whether a pair of zeros has appeared */

    while (remaining > 0) {
        /* Count completions if we put a 0 next. */
        idx_t new_has0 = has00 | (prev == 0);
        uint64_t count0 = dp[remaining - 1][0][new_has0];

        if (within < count0) {
            /* Choose 0. */
            value = (value << 1);
            has00 = new_has0;
            prev = 0;
        } else {
            /* The remaining strings begin with 1, which is allowed only if prev==0. */
            within -= count0;
            if (prev == 0) {
                /* Place 1. */
                value = (value << 1) | 1;
                /* has00 unchanged; prev becomes 1. */
                prev = 1;
            } else {
                /* No valid continuation – should not occur with proper `len`. */
                return 0;
            }
        }
        --remaining;
    }

    return (seq_t)value;
}
