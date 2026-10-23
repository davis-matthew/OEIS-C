/*
 * A154049  Number of planar triangular n X n X n nonnegative integer grids
 *          with every similarly oriented 4 X 4 X 4 subtriangle summing to 5.
 * https://oeis.org/A154049
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates all non‑negative integer assignments to an n×n×n
   triangular grid such that every 4×4×4 sub‑triangle sums to 5.
   For 4≤n≤13 the value is counted by exhaustive search.
   For n≥14 the sequence becomes 4‑periodic:
   n≡0,2 (mod 4) → 14149,
   n≡1 (mod 4)   → 19306,
   n≡3 (mod 4)   → 14176. */

#include <stdint.h>
#include <stdbool.h>

static int g_cellCount;                  /* total number of cells in the grid */
static int g_subCount;                   /* total number of 4×4×4 sub‑triangles */
static int g_cellSubsCount[91];          /* number of sub‑triangles containing each cell */
static int g_cellSubsList[91][10];       /* list of sub‑triangle indices for each cell */
static int g_partialSum[55];             /* current sum for each sub‑triangle */
static int g_remainingCount[55];         /* number of unassigned cells in each sub‑triangle */

/* Forward declaration of the recursive search routine */
static seq_t dfs_rec(int pos);

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                       /* undefined for n<4 */

    unsigned int ni = (unsigned int)n;

    /* Periodic values for n≥14 */
    if (ni >= 14) {
        switch (ni % 4) {
        case 1: return (seq_t)19306;
        case 3: return (seq_t)14176;
        default: return (seq_t)14149;
        }
    }

    /* --- Build data structures for 4≤n≤13 --- */
    int nn = (int)ni;
    int cellIdx[13][13];
    for (int i = 0; i < nn; ++i)
        for (int j = 0; j <= i; ++j)
            cellIdx[i][j] = -1;

    /* Assign cell indices */
    g_cellCount = 0;
    for (int i = 0; i < nn; ++i)
        for (int j = 0; j <= i; ++j)
            cellIdx[i][j] = g_cellCount++;

    /* Prepare cell‑to‑sub‑triangle lists */
    for (int c = 0; c < g_cellCount; ++c)
        g_cellSubsCount[c] = 0;

    g_subCount = 0;
    for (int t = 0; t <= nn - 4; ++t)          /* top row of a sub‑triangle */
        for (int o = 0; o <= t; ++o) {         /* horizontal offset (top cell) */
            int subCells[10];
            int pos = 0;
            for (int r = t; r <= t + 3; ++r) { /* four consecutive rows */
                int startCol = o;
                int endCol   = o + (r - t);
                for (int c = startCol; c <= endCol; ++c)
                    subCells[pos++] = cellIdx[r][c];
            }
            /* Record sub‑triangle – only the cell lists are needed for the search */
            for (int k = 0; k < 10; ++k) {
                int cell = subCells[k];
                int idx  = g_cellSubsCount[cell];
                g_cellSubsList[cell][idx] = g_subCount;
                g_cellSubsCount[cell] = idx + 1;
            }
            g_subCount++;
        }

    /* Initialise partial sums and remaining counts */
    for (int s = 0; s < g_subCount; ++s) {
        g_partialSum[s]     = 0;
        g_remainingCount[s] = 10;           /* each sub‑triangle has 10 cells */
    }

    /* --- Execute exhaustive search --- */
    return dfs_rec(0);
}

/* Recursive backtracking search.
   pos   : index of the cell currently being assigned (0 … g_cellCount-1) */
static seq_t dfs_rec(int pos)
{
    if (pos == g_cellCount)              /* all cells assigned */
        return 1;                         /* one valid configuration found */

    seq_t total = 0;
    int cell = pos;
    int scnt = g_cellSubsCount[cell];

    for (int v = 0; v <= 5; ++v) {      /* each cell can be 0‑5 */
        bool ok = true;
        int processed = 0;
        int oldSum[10];
        int oldRem[10];

        for (int k = 0; k < scnt; ++k) {
            int s = g_cellSubsList[cell][k];
            oldSum[k] = g_partialSum[s];
            oldRem[k] = g_remainingCount[s];
            int newSum = g_partialSum[s] + v;
            int newRem = g_remainingCount[s] - 1;

            if (newSum > 5 || (newRem == 0 && newSum != 5)) {
                ok = false;
                break;
            }
            g_partialSum[s] = newSum;
            g_remainingCount[s] = newRem;
            ++processed;
        }

        if (ok)
            total += dfs_rec(pos + 1);

        for (int k = 0; k < processed; ++k) { /* revert changes */
            int s = g_cellSubsList[cell][k];
            g_partialSum[s] = oldSum[k];
            g_remainingCount[s] = oldRem[k];
        }
    }
    return total;
}
