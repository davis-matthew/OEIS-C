/*
 * A234438  Number of (n+1) X (3+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×4 arrays of digits 0..2 such that every * 2×2
 *            subblock has its main‑diagonal sum differing from its
 *            antidiagonal * sum by exactly 1. No simple closed form is known;
 *            the values are * computed by dynamic programming over rows
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Number of possible rows: each row has 4 cells, each 0,1,2. */
#define ROWS 81

/* Compatibility: for a target row j, compatPrev[j] lists all rows i that
 * can immediately precede j (i.e. the 2×2 block formed by rows i and j
 * satisfies the diagonal−antidiagonal difference = 1 in all three
 * horizontal positions).  The maximum number of such predecessors per
 * row is at most ROWS, so we statically allocate a 2‑D array. */
static int compatPrev[ROWS][ROWS];
static int compatPrevCount[ROWS] = {0};

/* Flag indicating that the compatibility lists have been built already. */
static int comp_built = 0;

/* Build the compatibility matrix once (idempotent). */
static void build_compatibility(void)
{
    if (comp_built) return;
    comp_built = 1;

    /* Decode each row into its four ternary digits. */
    int rowDigits[ROWS][4];
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int k = 3; k >= 0; --k) {
            rowDigits[r][k] = v % 3;
            v /= 3;
        }
    }

    /* For every ordered pair (i,j) check the 3 horizontal 2×2 blocks. */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int ok = 1;
            for (int col = 0; col < 3; ++col) {
                int d_main   = rowDigits[i][col]     + rowDigits[j][col+1];
                int d_anti   = rowDigits[i][col+1]   + rowDigits[j][col];
                int diff = d_main > d_anti ? d_main - d_anti : d_anti - d_main;
                if (diff != 1) { ok = 0; break; }
            }
            if (ok) {
                int pos = compatPrevCount[j];
                compatPrev[j][pos] = i;
                compatPrevCount[j] = pos + 1;
            }
        }
    }
}

/* Compute the sequence value a(n).  The function is safe for any
 * sequence element type seq_t (8‑bit, 16‑bit, 32‑bit, 64‑bit, 128‑bit,
 * or arbitrary‑precision).  For large n the values may overflow small
 * types, but in those builds only terms that fit are checked. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; below offset undefined */

    /* Ensure compatibility one‑time. */
    build_compatibility();

    /* DP arrays: cur holds counts for the current row length,
     * initially length 1 (one row).  The values are held in seq_t.
     */
    static seq_t cur[ROWS];
    seq_t  next[ROWS];

    /* Initial vector: 1 for every possible first row. */
    for (int i = 0; i < ROWS; ++i) cur[i] = 1;

    /* Perform n transitions: after the loop we have counts for length n+1 rows. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next vector. */
        for (int j = 0; j < ROWS; ++j) next[j] = 0;

        /* Transition using compatibility lists. */
        for (int j = 0; j < ROWS; ++j) {
            int cnt = compatPrevCount[j];
            seq_t sum = 0;
            for (int k = 0; k < cnt; ++k) {
                int i = compatPrev[j][k];
                sum += cur[i];
            }
            next[j] = sum;
        }

        /* Move next to cur for the next iteration. */
        for (int i = 0; i < ROWS; ++i) cur[i] = next[i];
    }

    /* Total number of arrays: sum over all possible last rows. */
    seq_t total = 0;
    for (int i = 0; i < ROWS; ++i) total += cur[i];

    return total;
}
