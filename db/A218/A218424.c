/*
 * A218424  Number of n X 6 binary arrays with every 0 a horizontal, vertical
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A218424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n × 6 binary arrays such that each 0 has at least one
 *            horizontal, vertical or antidiagonal 1‑neighbour. The
 *            implementation uses a dynamic programme over triples of
 *            consecutive rows (〈row‑2,row‑1,row〉) and keeps the two most
 *            recent rows as a state. The transition matrix is pre‑computed
 *            o...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper macro to cast literals to seq_t when necessary */
#define SEQ_0 ((seq_t)0)
#define SEQ_1 ((seq_t)1)

/* Precomputed mask of valid next rows for a given pair (prevprev, prev) */
static uint64_t validMask[64][64];
static bool masks_prepared = false;

/* Check whether a given triple (a,b,c) of 6‑bit rows satisfies the
   neighbour condition for all 0‑cells in row b.  This function is used
   to build the transition masks. */
static bool is_valid_triple(int a, int b, int c)
{
    for (int col = 0; col < 6; ++col) {
        /* If the cell in b is 1, no constraint */
        if ((b >> col) & 1) continue;

        bool neigh = false;
        /* horizontal neighbours in the same row b */
        if (col > 0   && ((b >> (col - 1)) & 1)) neigh = true;
        if (col < 5   && ((b >> (col + 1)) & 1)) neigh = true;
        /* vertical neighbours */
        if ((a >> col) & 1) neigh = true;          /* up in row a */
        if ((c >> col) & 1) neigh = true;          /* down in row c */
        /* antidiagonal neighbours */
        if (col < 5   && ((a >> (col + 1)) & 1)) neigh = true; /* up-right */
        if (col > 0   && ((c >> (col - 1)) & 1)) neigh = true; /* down-left */

        if (!neigh) return false; /* isolated 0 */
    }
    return true;
}

/* Build the transition masks once */
static void prepare_masks(void)
{
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            uint64_t mask = 0;
            for (int c = 0; c < 64; ++c) {
                if (is_valid_triple(a, b, c))
                    mask |= ((uint64_t)1 << c);
            }
            validMask[a][b] = mask;
        }
    }
    masks_prepared = true;
}

seq_t a(idx_t n)
{
    if (!masks_prepared) prepare_masks();

    /* DP tables: dp[a][b] holds the number of ways to realise rows
       up to the current position, where the last two rows are a,b.   */
    static seq_t dp[64][64];
    static seq_t newdp[64][64];

    /* initialise to zero */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = SEQ_0;

    /* first row: prevprev is virtually all‑zeros (index 0) */
    for (int p = 0; p < 64; ++p)
        dp[0][p] = SEQ_1;

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* zero newdp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                newdp[i][j] = SEQ_0;

        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t val = dp[a][b];
                if (val == SEQ_0) continue;

                uint64_t m = validMask[a][b];
                while (m) {
                    int c = __builtin_ctzll(m);
                    newdp[b][c] += val;
                    m &= m - 1;
                }
            }
        }
        /* move newdp to dp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* After processing all rows, the last row is 'b'.
       Validate that every zero in this last row has a neighbour
       from row a (prevprev) or row b itself; there is no row below. */
    seq_t result = SEQ_0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            seq_t val = dp[a][b];
            if (val == SEQ_0) continue;

            bool ok = true;
            for (int col = 0; col < 6; ++col) {
                if ((b >> col) & 1) continue;

                bool neigh = false;
                if (col > 0   && ((b >> (col - 1)) & 1)) neigh = true;
                if (col < 5   && ((b >> (col + 1)) & 1)) neigh = true;
                if ((a >> col) & 1) neigh = true;
                if (col < 5   && ((a >> (col + 1)) & 1)) neigh = true;

                if (!neigh) { ok = false; break; }
            }
            if (ok) result += val;
        }
    }
    return result;
}
