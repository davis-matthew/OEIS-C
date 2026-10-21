/*
 * A201730  Triangle T(n,k), read by rows, given by
 *          (2,1/2,3/2,0,0,0,0,0,0,0,...) DELTA (0,1/2,-1/2,0,0,0,0,0,0,0,...)
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A201730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,0)=1, T(1,0)=2, T(1,1)=0. For n ≥ 2 and 0 ≤ k < n T(n,k) =
 *            4*T(n‑1,k) + T(n‑2,k‑1) – 3*T(n‑2,k) T(n,n)=0 for n≥1. a(n) is
 *            the element of the triangle read row‑by‑row
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **ROWS = (seq_t**)0;      /* ROWS[r][k] stores T(r,k) */
static size_t MAX_ROW = 0;            /* highest row index that has been computed */

/* Return the largest row r such that r*(r+1)/2 ≤ n. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (1) {
        idx_t next = (r + 1) * (r + 2) / 2;
        if (next > n) break;
        r++;
    }
    return r;
}

/* Allocate rows 0 … r and store them in ROWS.  Idempotent. */
static void ensure_rows(idx_t r)
{
    if (ROWS == (seq_t**)0) {
        ROWS = (seq_t**)calloc(1, sizeof(seq_t*));
        if (ROWS == (seq_t**)0) return;      /* allocation failure */

        seq_t *row0 = (seq_t*)calloc(1, sizeof(seq_t));
        if (row0 == (seq_t*)0) { free(ROWS); ROWS = (seq_t**)0; return; }
        row0[0] = 1;
        ROWS[0] = row0;
        MAX_ROW = 0;
    }

    if (MAX_ROW >= r) return;

    if (r + 1 > (idx_t)(MAX_ROW + 1)) {
        ROWS = (seq_t**)realloc(ROWS, (r + 1) * sizeof(seq_t*));
        if (ROWS == (seq_t**)0) return;
    }

    for (idx_t i = MAX_ROW + 1; i <= r; ++i) {
        size_t len = (size_t)(i + 1);
        seq_t *row = (seq_t*)calloc(len, sizeof(seq_t));
        if (row == (seq_t*)0) return;

        if (i == 1) {
            row[0] = 2;
            row[1] = 0;
        } else if (i > 1) {
            seq_t *prev1 = ROWS[i - 1];
            seq_t *prev2 = ROWS[i - 2];
            for (size_t k = 0; k <= i; ++k) {
                if (k == i) {
                    row[k] = 0;
                } else {
                    seq_t val = (seq_t)4 * prev1[k];
                    if (k >= 1)
                        val += prev2[k - 1];
                    if (k <= (size_t)(i - 2))
                        val -= (seq_t)3 * prev2[k];
                    row[k] = val;
                }
            }
        }
        ROWS[i] = row;
    }
    MAX_ROW = r;
}

/* Main sequence function: returns a(n). */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    ensure_rows(r);
    return ROWS[r][k];
}
