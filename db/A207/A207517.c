/*
 * A207517  Number of nX6 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   is_horizontal_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A207517: number of n×6 0..1 arrays avoiding
 *  horizontally: 0 0 1 or 0 1 0
 *  vertically:   0 1 0 or 1 0 1
 *
 * The sequence is computed by dynamic programming on row states.
 * The state consists of two consecutive rows (previous two rows).
 * A row is represented as a 6‑bit integer.
 *
 * For each step we add a new row `c` that satisfies:
 *   1) `c` itself has no horizontal forbidden pattern,
 *   2) for each column where the two previous rows differ
 *      the new row must equal the previous row in that column
 *      (to avoid vertical 010 or 101).
 *
 * The DP runs in O(n * |H|^2) where |H| is the number of horizontal
 * valid rows (|H|=21).  This easily meets the time constraints.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_horizontal_valid.h"

seq_t a(idx_t n)
{
    /* Enumerate all rows that satisfy horizontal constraints. */
    uint8_t rows[64];
    size_t M = 0;
    for (uint8_t r = 0; r < 64; ++r)
    {
        if (is_horizontal_valid(r))
        {
            rows[M++] = r;
        }
    }

    /* Handle small n explicitly. */
    if (n == 1)
    {
        return (seq_t)M;
    }
    if (n == 2)
    {
        return (seq_t)M * (seq_t)M;
    }

    /* DP over pairs of previous rows. */
    static seq_t dp[64][64];
    static seq_t newdp[64][64];

    /* Initialise dp for length 2: every pair is allowed. */
    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            dp[i][j] = 1;
        }
    }

    /* Transition for rows 3 .. n. */
    for (idx_t step = 3; step <= n; ++step)
    {
        /* zero newdp */
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < M; ++j)
            {
                newdp[i][j] = 0;
            }
        }

        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < M; ++j)
            {
                seq_t cur = dp[i][j];
                if (cur == 0)
                    continue;
                uint8_t a_row = rows[i];
                uint8_t b_row = rows[j];
                /* Try all possible next rows c. */
                for (size_t k = 0; k < M; ++k)
                {
                    uint8_t c_row = rows[k];
                    bool ok = true;
                    for (int pos = 0; pos < 6; ++pos)
                    {
                        bool diff = (((a_row >> pos) & 1U) !=
                                     ((b_row >> pos) & 1U));
                        if (diff && (((c_row >> pos) & 1U) !=
                                     ((b_row >> pos) & 1U)))
                        {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                    {
                        newdp[j][k] += cur;
                    }
                }
            }
        }

        /* swap dp and newdp */
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < M; ++j)
            {
                dp[i][j] = newdp[i][j];
            }
        }
    }

    /* Sum over all pairs to get total number of sequences of length n. */
    seq_t total = 0;
    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            total += dp[i][j];
        }
    }
    return total;
}
