/*
 * A232118  Number of (n+1)X(2+1) 0..2 arrays with every element both >= and
 *          <= some horizontal or vertical neighbor.
 * https://oeis.org/A232118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..2 such that *
 *            each entry has at least one neighbor (horizontal or vertical)
 *            that is ≤ * it and at least one neighbor that is ≥ it. The
 *            computation is done * by dynamic programming over the rows. The
 *            DP state stores, for a * given row, which columns...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Matrices for pattern preprocessing – constant dimensions 27×27×... */
#define PAT 27      /* 3³ possible row patterns */
#define MASKS 64    /* 2⁶ possible deficit masks */

/* Row pattern data */
static uint8_t pat_digits[PAT][3];
static uint8_t hmask[PAT];
static uint8_t newmask[PAT][PAT];
static uint64_t compat[PAT][PAT];
static bool init_done = false;

/* Precompute all pattern-dependent tables once. */
static void init_patterns(void)
{
    /* Decode each pattern into its three digits */
    for (int p = 0; p < PAT; ++p) {
        int x = p;
        pat_digits[p][2] = x % 3; x /= 3;
        pat_digits[p][1] = x % 3; x /= 3;
        pat_digits[p][0] = x % 3;
    }

    /* Compute horizontal deficit masks for each pattern. */
    for (int p = 0; p < PAT; ++p) {
        uint8_t m = 0;
        for (int j = 0; j < 3; ++j) {
            int val = pat_digits[p][j];
            int low  = 0, high = 0;
            if (j > 0 && pat_digits[p][j-1] <= val) low = 1;
            if (j < 2 && pat_digits[p][j+1] <= val) low = 1;
            if (j > 0 && pat_digits[p][j-1] >= val) high = 1;
            if (j < 2 && pat_digits[p][j+1] >= val) high = 1;
            if (!low)  m |= 1u << j;           /* low unsatisfied bit */
            if (!high) m |= 1u << (3 + j);     /* high unsatisfied bit */
        }
        hmask[p] = m;
    }

    /* Precompute newmask and compatibility tables. */
    for (int p = 0; p < PAT; ++p) {
        for (int q = 0; q < PAT; ++q) {
            /* New deficits after vertical neighbours from p to q */
            uint8_t nm = hmask[q];
            for (int j = 0; j < 3; ++j) {
                if (pat_digits[p][j] <= pat_digits[q][j]) /* low satisfied */
                    nm &= ~(1u << j);
                if (pat_digits[p][j] >= pat_digits[q][j]) /* high satisfied */
                    nm &= ~(1u << (3 + j));
            }
            newmask[p][q] = nm;

            /* Compatibility: for each deficit mask m from row p,
             * check if row q can satisfy those deficits via its
             * vertical neighbour p.  The result is stored as a bitset
             * of all compatible m values. */
            uint64_t cm = 0;
            for (int m = 0; m < MASKS; ++m) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    if (m & (1u << j)) {               /* low needed */
                        if (pat_digits[q][j] > pat_digits[p][j]) { ok = false; break; }
                    }
                    if (m & (1u << (3 + j))) {         /* high needed */
                        if (pat_digits[q][j] < pat_digits[p][j]) { ok = false; break; }
                    }
                }
                if (ok) cm |= 1ull << m;
            }
            compat[p][q] = cm;
        }
    }
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 – undefined for n=0 */

    if (!init_done) { init_patterns(); init_done = true; }

    /* DP arrays: cur[p][m] = number of ways to reach a row with
     * pattern p and deficit mask m.  Size is constant (27×64). */
    seq_t cur[PAT][MASKS];
    seq_t nxt[PAT][MASKS];

    /* Initialise zero */
    for (int p = 0; p < PAT; ++p)
        for (int m = 0; m < MASKS; ++m) cur[p][m] = 0;

    /* First row: deficits are exactly the horizontal ones. */
    for (int p = 0; p < PAT; ++p)
        cur[p][hmask[p]] = 1;

    /* Process the remaining n rows (total rows = n+1). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero next array */
        for (int p = 0; p < PAT; ++p)
            for (int m = 0; m < MASKS; ++m) nxt[p][m] = 0;

        for (int p = 0; p < PAT; ++p) {
            for (int pm = 0; pm < MASKS; ++pm) {
                seq_t cnt = cur[p][pm];
                if (cnt == 0) continue;
                for (int q = 0; q < PAT; ++q) {
                    if ((compat[p][q] & (1ull << pm)) == 0) continue;
                    uint8_t nm = newmask[p][q];
                    nxt[q][nm] += cnt;
                }
            }
        }

        /* Swap arrays for next iteration. */
        for (int p = 0; p < PAT; ++p)
            for (int m = 0; m < MASKS; ++m)
                cur[p][m] = nxt[p][m];
    }

    /* Sum all states where the final row has no unsatisfied deficits. */
    seq_t ans = 0;
    for (int p = 0; p < PAT; ++p)
        ans += cur[p][0];

    return ans;
}
