/*
 * A207783  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A207783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 18, a(2) = 324, a(n) = Σ_{i,j,k} 1_{allowed(i,j,k)}·count(i,j) for n≥3 */
/* The values are computed by dynamic programming over row-representations. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Build‑time constants ----------------------------------------------------- */
#define COLS        6
#define MAX_ROWS    18                  /* number of admissible 6‑bit rows          */
#define STATECOUNT  (MAX_ROWS * MAX_ROWS)

/* 1. Pre‑computed row masks and transition table -------------------------- */
static int rows[MAX_ROWS];                                   /* all admissible row masks */
static int allowed[MAX_ROWS][MAX_ROWS][MAX_ROWS];           /* allowed(row_i,row_j,row_k) */

/* 2. DP buffers ----------------------------------------------------------- */
static seq_t curr[STATECOUNT];
static seq_t nxt [STATECOUNT];

/* 3. Memoized result table ---------------------------------------------- */
static seq_t *totals = NULL;   /* totals[n] = a(n)  (for 0 ≤ n < totals_cap) */
static size_t totals_cap = 0;  /* allocated capacity */
static idx_t  totals_len  = 0; /* next free index */

/* 4. Initialise the data structures once --------------------------------- */
static void init_sequence(void)
{
    /* -- build the list of admissible rows -------------------------------- */
    int rc = 0;
    for (int w = 0; w < (1 << COLS); ++w) {
        bool ok = true;
        for (int p = 0; p < COLS - 2; ++p) {
            int a = (w >> (p + 0)) & 1;
            int b = (w >> (p + 1)) & 1;
            int c = (w >> (p + 2)) & 1;
            if (a == 0 && b == 0 && c == 0) { ok = false; break; }
            if (a == 0 && b == 1 && c == 1) { ok = false; break; }
        }
        if (ok) rows[rc++] = w;
    }
    /* rc should be 18 for this problem */

    /* -- pre‑compute allowed transitions --------------------------------- */
    for (int i = 0; i < rc; ++i) {
        int w1 = rows[i];
        for (int j = 0; j < rc; ++j) {
            int w2 = rows[j];
            for (int k = 0; k < rc; ++k) {
                int w3 = rows[k];
                bool ok = true;
                for (int col = 0; col < COLS; ++col) {
                    int b2 = (w1 >> col) & 1;
                    int b1 = (w2 >> col) & 1;
                    int b0 = (w3 >> col) & 1;
                    if (b2 == 0 && b1 == 0 && b0 == 0) { ok = false; break; }
                    if (b2 == 1 && b1 == 0 && b0 == 1) { ok = false; break; }
                }
                allowed[i][j][k] = ok;
            }
        }
    }

    /* -- initialise memoised table ---------------------------------------- */
    totals_cap = 8;              /* initial capacity */
    totals     = (seq_t *)malloc(totals_cap * sizeof(seq_t));
    if (!totals) exit(EXIT_FAILURE);

    totals[0] = 0;                               /* a(0) – unused but for safety   */
    totals[1] = (seq_t)rc;                      /* a(1) = #rows                    */
    totals[2] = (seq_t)rc * (seq_t)rc;          /* a(2) = rows²                     */
    totals_len = 3;                             /* next free index  */

    /* -- initialise DP buffers for length 2 tables ------------------------ */
    for (size_t idx = 0, max = STATECOUNT; idx < max; ++idx)
        curr[idx] = 1;                          /* every (row_i,row_j) is admissible */

    /* no global state needed beyond totals_len */
}

/* 5. Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialised = false;
    if (!initialised) { init_sequence(); initialised = true; }

    if (n == 0) return 0;                     /* offset forbids n=0, but safe */

    /* return memoised value if already computed */
    if (n < totals_len) return totals[n];

    /* enlarge memoisation table if necessary */
    if ((size_t)n >= totals_cap) {
        size_t new_cap = totals_cap * 2 + 10;
        seq_t *tmp = (seq_t *)realloc(totals, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        totals     = tmp;
        totals_cap = new_cap;
    }

    /* compute successive lengths up to n */
    for (idx_t len = totals_len; len <= n; ++len) {
        /* zero nxt buffer */
        for (size_t idx = 0; idx < STATECOUNT; ++idx) nxt[idx] = 0;

        /* extend all pairs by a third row */
        for (int i = 0; i < MAX_ROWS; ++i) {
            for (int j = 0; j < MAX_ROWS; ++j) {
                seq_t cnt = curr[i * MAX_ROWS + j];
                if (cnt == 0) continue;
                for (int k = 0; k < MAX_ROWS; ++k) {
                    if (!allowed[i][j][k]) continue;
                    nxt[j * MAX_ROWS + k] += cnt;
                }
            }
        }

        /* sum over all (j,k) to obtain a(len) */
        seq_t sum = 0;
        for (size_t idx = 0; idx < STATECOUNT; ++idx) sum += nxt[idx];
        totals[len] = sum;
        ++totals_len;

        /* prepare for next iteration */
        for (size_t idx = 0; idx < STATECOUNT; ++idx)
            curr[idx] = nxt[idx];
    }

    return totals[n];
}
