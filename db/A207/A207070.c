/*
 * A207070  Number of 3Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207070 – number of 3×n 0..1 arrays avoiding
 *  the horizontal patterns 001 and 010 and the vertical patterns 001 and 101.
 *  Computed via a linear-time DP over the last two columns.
 *  The DP state encodes, for each of the three rows, the two
 *  most recent bits.  There are 64 possible states.
 */

#include <stddef.h>
#include <stdint.h>

static const int allowed_patterns[6] = {
    0b000, /* 000 */
    0b010, /* 010 */
    0b011, /* 011 */
    0b100, /* 100 */
    0b110, /* 110 */
    0b111  /* 111 */
};

/* bit positions for rows: top=2, mid=1, bottom=0 */
static const int row_bit_pos[3] = {2, 1, 0};

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n < 1)
        return (seq_t)0;

    /* n = 1 and n = 2 have trivial answers */
    if (n == 1)
        return (seq_t)6;  /* 6 allowed columns */
    if (n == 2)
        return (seq_t)36; /* 6×6 possible 2‑column pairs */

    /* DP arrays: dp[s] – number of valid arrays of the current length
     * whose last two columns produce state s.  States are 6‑bit values. */
    seq_t dp[64]   = {0};
    seq_t newdp[64] = {0};

    /* Initialise dp for length 2 (two columns) – all patterns allowed. */
    for (int i = 0; i < 6; ++i) {
        int c1 = allowed_patterns[i];
        for (int j = 0; j < 6; ++j) {
            int c2 = allowed_patterns[j];
            int s = 0;
            for (int r = 0; r < 3; ++r) {
                int bit1 = (c1 >> row_bit_pos[r]) & 1; /* column 1 */
                int bit2 = (c2 >> row_bit_pos[r]) & 1; /* column 2 */
                s |= bit1 << (2 * r);                 /* prevprev */
                s |= bit2 << (2 * r + 1);             /* prev    */
            }
            dp[s] += (seq_t)1;
        }
    }

    /* Process columns 3 … n */
    for (idx_t col = 3; col <= n; ++col) {
        /* reset newdp */
        for (int k = 0; k < 64; ++k)
            newdp[k] = (seq_t)0;

        for (int s = 0; s < 64; ++s) {
            seq_t ways = dp[s];
            if (ways == (seq_t)0)
                continue;

            /* Try every allowed column pattern */
            for (int p_index = 0; p_index < 6; ++p_index) {
                int p = allowed_patterns[p_index];
                int ns = 0;          /* next state */
                int ok = 1;          /* pattern validity flag */

                for (int r = 0; r < 3; ++r) {
                    int prevprev = (s >> (2 * r)) & 1;
                    int prev     = (s >> (2 * r + 1)) & 1;
                    int curnew   = (p >> row_bit_pos[r]) & 1;

                    /* Check forbidden triple 001 and 010 */
                    if (!((prevprev == 0 && prev == 0 && curnew == 1) ||
                          (prevprev == 0 && prev == 1 && curnew == 0))) {
                        /* Update next state */
                        ns |= prev << (2 * r);          /* new prevprev = old prev */
                        ns |= curnew << (2 * r + 1);    /* new prev     = curnew */
                    } else {
                        ok = 0;
                        break;
                    }
                }

                if (ok) {
                    newdp[ns] += ways;
                }
            }
        }

        /* Prepare for next iteration */
        for (int k = 0; k < 64; ++k)
            dp[k] = newdp[k];
    }

    /* Sum all DP states to get the total count for length n */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 64; ++s)
        total += dp[s];

    return total;
}
