/*
 * A220642  Number of ways to reciprocally link elements of an n X 6 array
 *          either to themselves or to exactly one king-move neighbor.
 * https://oeis.org/A220642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A220642
   Number of ways to reciprocally link elements of an n x 6 array
   either to themselves or to exactly one king-move neighbor.
   Recurrence in terms of matchings of the king-move graph.
   The algorithm constructs the transfer matrix for one row and
   performs DP over rows.  The missing constraint is that two
   different cells in the same row cannot both match to the same
   cell in the next row; this is enforced by checking mask_out
   before placing a vertical or a diagonal edge. */

#include <stdbool.h>

static seq_t TRANS[64][64];

/* Recursive enumeration of all possible matchings for a single row.
   pos       : current column (0‑5)
   unfilled  : bitmask of still‑unused cells in this row
   mask_out  : bitmask of cells in the next row that are matched to
               cells in the current row
   mask_in   : bitmask of cells already matched to the previous row   */
static void rec_row(int pos, idx_t unfilled, idx_t mask_out, idx_t mask_in)
{
    if (pos == 6) {
        TRANS[mask_in][mask_out] += 1;
        return;
    }

    /* Skip cells that are already matched from above. */
    if (!(unfilled & (1u << pos))) {
        rec_row(pos + 1, unfilled, mask_out, mask_in);
        return;
    }

    /* 1. self‑loop (cell remains unmatched). */
    rec_row(pos + 1,
            unfilled & ~(1u << pos),
            mask_out,
            mask_in);

    /* 2. horizontal match to the right neighbour. */
    if (pos + 1 < 6 && (unfilled & (1u << (pos + 1)))) {
        rec_row(pos + 1,
                unfilled & ~(1u << pos) & ~(1u << (pos + 1)),
                mask_out,
                mask_in);
    }

    /* 3. vertical match downwards (current cell matched to next row). */
    if (!(mask_out & (1u << pos))) { /* ensure the target cell is free */
        rec_row(pos + 1,
                unfilled & ~(1u << pos),
                mask_out | (1u << pos),
                mask_in);
    }

    /* 4. diagonal down‑left. */
    if (pos > 0 && !(mask_out & (1u << (pos - 1)))) {
        rec_row(pos + 1,
                unfilled & ~(1u << pos),
                mask_out | (1u << (pos - 1)),
                mask_in);
    }

    /* 5. diagonal down‑right. */
    if (pos + 1 < 6 && !(mask_out & (1u << (pos + 1)))) {
        rec_row(pos + 1,
                unfilled & ~(1u << pos),
                mask_out | (1u << (pos + 1)),
                mask_in);
    }
}

static void build_transitions(void)
{
    /* initialise the transition table */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            TRANS[i][j] = 0;

    const idx_t FULL = (idx_t)((1u << 6) - 1);
    for (idx_t mask_in = 0; mask_in <= FULL; ++mask_in) {
        idx_t unfilled = FULL ^ mask_in;   /* cells not yet matched to above */
        rec_row(0, unfilled, 0, mask_in);
    }
}

seq_t a(idx_t n)
{
    static int built = 0;
    if (!built) {
        build_transitions();
        built = 1;
    }

    /* DP arrays for current and next row */
    static seq_t dp_cur[64];
    static seq_t dp_next[64];
    const idx_t FULL = (idx_t)((1u << 6) - 1);

    /* initialise dp_cur */
    for (int i = 0; i < 64; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1; /* no pending matches before the first row */

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;
        for (int mask_in = 0; mask_in <= FULL; ++mask_in) {
            if (dp_cur[mask_in] == 0) continue;
            for (int mask_out = 0; mask_out <= FULL; ++mask_out) {
                seq_t t = TRANS[mask_in][mask_out];
                if (t == 0) continue;
                dp_next[mask_out] += dp_cur[mask_in] * t;
            }
        }
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
    }

    return dp_cur[0]; /* all pending matches must be resolved */
}
