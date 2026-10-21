/*
 * A238930  Number of (1+1)X(n+1) 0..3 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A238930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence computed by a transfer‑matrix * counting 2×(n+1) arrays
 *            with entries 0..3, no element greater * than all horizontal
 *            neighbors, and no element equal to its vertical neighbor. * *
 *            The DP state stores two consecutive columns (previous and
 *            current). * Transition checks that the middle column...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* number of distinct 2‑row columns with two different values in 0..3 */
#define COLS 12
/* number of possible (previous column, current column) states */
#define STATE (COLS * COLS)

/* data for a column: top and bottom values */
static int column[COLS][2];

/* precomputed transitions for a state (p,l) → r (new column) */
static int next[STATE][COLS];
static unsigned char next_cnt[STATE];

/* DP tables */
static seq_t dp[STATE];
static seq_t ndp[STATE];

/* flag to create tables once */
static bool ready = false;

/* initialise column patterns and transition tables */
static void init_tables(void)
{
    if (ready) return;
    /* construct all 12 columns (a,b) with a!=b */
    int idx = 0;
    for (int a = 0; a <= 3; ++a)
        for (int b = 0; b <= 3; ++b)
            if (a != b) {
                column[idx][0] = a;        /* top  */
                column[idx][1] = b;        /* bottom */
                ++idx;
            }

    /* precompute transitions */
    for (int p = 0; p < COLS; ++p) {
        for (int l = 0; l < COLS; ++l) {
            int s = p * COLS + l;          /* state index */
            int cnt = 0;
            for (int r = 0; r < COLS; ++r) {
                /* l cannot be strictly greater than both p and r in any row */
                bool bad = false;
                for (int row = 0; row < 2; ++row) {
                    if (column[l][row] > column[p][row] &&
                        column[l][row] > column[r][row]) {
                        bad = true;
                        break;
                    }
                }
                if (!bad) {
                    next[s][cnt++] = r;
                }
            }
            next_cnt[s] = (unsigned char)cnt;
        }
    }
    ready = true;
}

/* the required function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* the sequence is defined for n ≥ 1 */

    init_tables();

    /* -------- initialise DP for the first two columns ---------------- */

    /* reset dp */
    for (int i = 0; i < STATE; ++i) dp[i] = 0;

    /* first column has no left neighbour → it must be ≤ second column in each row */
    for (int p = 0; p < COLS; ++p) {
        for (int l = 0; l < COLS; ++l) {
            bool ok = true;
            for (int row = 0; row < 2; ++row) {
                if (column[p][row] > column[l][row]) {
                    ok = false; break;
                }
            }
            if (ok) {
                int s = p * COLS + l;
                dp[s] = 1;
            }
        }
    }

    /* -------- DP for columns 2 .. n --------------------------------- */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear ndp */
        for (int i = 0; i < STATE; ++i) ndp[i] = 0;

        for (int s = 0; s < STATE; ++s) {
            seq_t val = dp[s];
            if (!val) continue;

            int l = s % COLS;              /* current column index */
            int cnt = next_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int r = next[s][k];
                int ns = l * COLS + r;      /* new state (l,r) */
                ndp[ns] += val;
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < STATE; ++i) dp[i] = ndp[i];
    }

    /* -------- final boundary: last column must be ≤ its left neighbour -------
       For n==1 this loop is skipped, but we need to enforce that column 0
       ≤ column 1 and column 1 ≤ column 0 → equality.  Thus we handle
       n==1 as a special case. */
    seq_t total = 0;
    if (n == 1) {
        for (int s = 0; s < STATE; ++s) {
            int p = s / COLS;
            int l = s % COLS;
            bool equal = true;
            for (int row = 0; row < 2; ++row) {
                if (column[p][row] != column[l][row]) { equal = false; break; }
            }
            if (equal) total += dp[s];
        }
    } else {
        /* For the final two columns (p = col_{n-1}, l = col_n),
           enforce that l ≤ p in each row. */
        for (int s = 0; s < STATE; ++s) {
            int p = s / COLS;
            int l = s % COLS;
            bool ok = true;
            for (int row = 0; row < 2; ++row) {
                if (column[l][row] > column[p][row]) { ok = false; break; }
            }
            if (ok) total += dp[s];
        }
    }

    return total;
}
