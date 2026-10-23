/*
 * A183364  Number of n X 2 binary arrays with no element equal to the sum mod
 *          3 of its horizontal and vertical neighbors.
 * https://oeis.org/A183364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A183364
   Number of n × 2 binary arrays with no element equal to the sum
   (mod 3) of its horizontal and vertical neighbors.
   Computed by dynamic programming over rows. */
#include <stdbool.h>

/* check that a single row is valid given its two vertical neighbours
   (up and down).  If a neighbour does not exist, the corresponding flag
   must be false and the neighbour value is ignored.  Row is a 2‑bit
   mask: bit 0 = column 0 (left), bit 1 = column 1 (right). */
static inline bool
row_valid(uint8_t row, uint8_t up, uint8_t down,
          bool has_up, bool has_down)
{
    for (int col = 0; col < 2; ++col) {
        int val = (row >> col) & 1;               /* current cell         */
        int hor = (row >> (col ^ 1)) & 1;         /* horizontal neighbour */
        int sum = hor;                            /* start with horizontal */

        if (has_up)  sum += (up >> col) & 1;      /* add up neighbour   */
        if (has_down) sum += (down >> col) & 1;   /* add down neighbour */

        /* forbidden if cell equals (sum mod 3) */
        if (val == (sum % 3))
            return false;
    }
    return true;
}

/* DP over rows: dp[prev][curr] holds the number of ways to build
   rows up to the current position with the last two rows = prev and curr. */
seq_t a(idx_t n)
{
    /* Offset is 1: for n <= 0 return 0 */
    if (n <= 0)
        return (seq_t)0;

    /* For n = 1, answer is 2 (rows 01 and 10) */
    if (n == 1)
        return (seq_t)2;

    /* Small fixed table – no variable length arrays */
    enum { M = 4 };                         /* 2 bits → 4 possible rows */
    seq_t dp[M][M] = {0};
    seq_t ndp[M][M] = {0};

    /* Initialise dp with all pairs (row1, row2) that satisfy the
       constraints for the first row (no up neighbor, one down neighbor). */
    for (int r1 = 0; r1 < M; ++r1) {
        for (int r2 = 0; r2 < M; ++r2) {
            if (row_valid((uint8_t)r1,
                           0, (uint8_t)r2,
                           false, true))
                dp[r1][r2] += (seq_t)1;
        }
    }

    /* Process rows 3 … n.  For each step i we keep in dp the states
       (row_{i-2}, row_{i-1}) and build ndp for states (row_{i-1}, row_i). */
    for (idx_t i = 3; i <= n; ++i) {
        /* clear ndp */
        for (int a = 0; a < M; ++a)
            for (int b = 0; b < M; ++b)
                ndp[a][b] = (seq_t)0;

        for (int a = 0; a < M; ++a)          /* a = row_{i-2} */
            for (int b = 0; b < M; ++b)      /* b = row_{i-1} */
                if (dp[a][b] != (seq_t)0) {
                    for (int c = 0; c < M; ++c) {   /* c = row_i */
                        if (row_valid((uint8_t)b,
                                       (uint8_t)a, (uint8_t)c,
                                       true, true))
                            ndp[b][c] += dp[a][b];
                    }
                }

        /* move ndp → dp */
        for (int a = 0; a < M; ++a)
            for (int b = 0; b < M; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Final check for the last row (row_n): has an up neighbour but no down. */
    seq_t ans = (seq_t)0;
    for (int a = 0; a < M; ++a)          /* a = row_{n-1} */
        for (int b = 0; b < M; ++b)      /* b = row_n   */
            if (row_valid((uint8_t)b,
                           (uint8_t)a, 0,
                           true, false))
                ans += dp[a][b];

    return ans;
}
