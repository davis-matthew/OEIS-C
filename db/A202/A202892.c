/*
 * A202892  Number of (n+2) X 4 binary arrays avoiding patterns 001 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+2) × 4 binary arrays avoiding * the patterns 001
 *            and 010 in rows, columns, and north‑west to * south‑east
 *            diagonals. The array is indexed 1‑based * (a(1) corresponds to
 *            3×4 arrays)
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The nine row patterns that contain no horizontal 001 or 010.
 * Bit 3 is the leftmost column. */
static const uint8_t ALLOWED_ROWS[9] =
    {0, 6, 7, 8, 11, 12, 13, 14, 15};

/* Transition tables: for every ordered pair (i,j) the list of
 * admissible next rows k (0..8).  The tables are built once
 * the first time a() is called. */
static uint8_t TRANS_NEXT[81][9];
static uint8_t TRANS_COUNT[81];
static bool    TRANS_INIT = false;

/* Return true if rows i, j, k satisfy the vertical and diagonal
 * pattern restrictions.  Bit indices are treated so that column 0
 * corresponds to bit 3 (most significant bit). */
static bool
valid_triple(int i, int j, int k)
{
    uint8_t a = ALLOWED_ROWS[i];
    uint8_t b = ALLOWED_ROWS[j];
    uint8_t c = ALLOWED_ROWS[k];

    /* Vertical checks: each column independently. */
    for (int col = 0; col < 4; ++col) {
        int shift = 3 - col;              /* bit for current column */
        int top    = (a >> shift) & 1;
        int mid    = (b >> shift) & 1;
        int bottom = (c >> shift) & 1;
        if ((top == 0 && mid == 0 && bottom == 1)   ||
            (top == 0 && mid == 1 && bottom == 0))
            return false;                 /* 001 or 010 occurs */
    }

    /* Diagonal checks for the two NW–SE diagonals of length 3. */
    for (int d = 0; d <= 1; ++d) {
        int shift0 = 3 - d;               /* a[d] */
        int shift1 = 3 - (d + 1);          /* b[d+1] */
        int shift2 = 3 - (d + 2);          /* c[d+2] */
        int top    = (a >> shift0) & 1;
        int mid    = (b >> shift1) & 1;
        int bottom = (c >> shift2) & 1;
        if ((top == 0 && mid == 0 && bottom == 1)   ||
            (top == 0 && mid == 1 && bottom == 0))
            return false;
    }
    return true;
}

/* Initialise the transition tables on first use. */
static void
init_transitions(void)
{
    if (TRANS_INIT) return;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int idx = i * 9 + j;
            int cnt = 0;
            for (int k = 0; k < 9; ++k) {
                if (valid_triple(i, j, k)) {
                    TRANS_NEXT[idx][cnt++] = (uint8_t)k;
                }
            }
            TRANS_COUNT[idx] = (uint8_t)cnt;
        }
    }
    TRANS_INIT = true;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Length of the array in rows is n+2. */
    idx_t rows = n + 2;
    if (rows < 3) return (seq_t)0;   /* unused for n>=1 */

    init_transitions();

    /* DP tables: curr holds counts for last pair (p,q); next for (q,r). */
    seq_t curr[81];
    seq_t next[81];
    for (int i = 0; i < 81; ++i) curr[i] = next[i] = (seq_t)0;

    /* Every ordered pair of initial two rows is valid. */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            curr[i * 9 + j] = (seq_t)1;

    /* Append rows 3 .. rows. */
    for (idx_t r = 2; r < rows; ++r) {
        for (int idx = 0; idx < 81; ++idx) {
            seq_t ways = curr[idx];
            if (!ways) continue;
            int last = idx % 9;                 /* second row of the pair */
            uint8_t *next_list = TRANS_NEXT[idx];
            uint8_t  next_cnt   = TRANS_COUNT[idx];
            for (int t = 0; t < next_cnt; ++t) {
                int k = next_list[t];
                int new_idx = last * 9 + k;
                next[new_idx] += ways;
            }
        }
        /* Move to next step. */
        for (int i = 0; i < 81; ++i) {
            curr[i] = next[i];
            next[i] = (seq_t)0;
        }
    }

    /* Sum all counts for the final row pairs. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 81; ++i) result += curr[i];
    return result;
}
