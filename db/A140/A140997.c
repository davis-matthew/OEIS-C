/*
 * A140997  Triangle G(n,k) read by rows, for 0 <= k <= n, where G(n, 0) =
 *          G(n+1, n+1) = 1, G(n+2, n+1) = 2, G(n+3, n+1) = 4, and G(n+4, m) =
 *          G(n+1, m-1) + G(n+1, m) + G(n+2, m) + G(n+3, m) for n >= 0 and m =
 *          1..n+1.
 * https://oeis.org/A140997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 172, u32 n <= 631, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle G(n,k) defined by
 *      G(n,0) = 1,
 *      G(n+1,n+1) = 1,
 *      G(n+2,n+1) = 2,
 *      G(n+3,n+1) = 4,
 *      G(n+4,m) = G(n+1,m-1) + G(n+1,m) + G(n+2,m) + G(n+3,m)
 *      for n ≥ 0, 1 ≤ m ≤ n+1.
 *  The sequence a(n) is the row‑wise flattening of G.  This function
 *  builds the triangle row by row, caching the flattened values so
 *  that subsequent calls are answered in O(1).  It is correct for
 *  any seq_t / idx_t types supplied by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum row length needed for n up to about 10000 */
#define MAX_ROW 200

/* circular buffer for the last four triangle rows */
static seq_t rows[4][MAX_ROW];
static idx_t row_len[4] = {0,0,0,0};

/* linear (flattened) cache of the triangle */
static seq_t *lin_cache = NULL;
static idx_t lin_len   = 0;
static size_t lin_cap  = 0;

/* index of the next row to compute (row number) */
static idx_t next_row   = 1;   /* row 0 already cached */

/* initialization flag */
static bool initialized = false;

/* Ensure the linear cache can hold at least 'need' elements. */
static void ensure_lin_cap(idx_t need)
{
    if ((size_t)need <= lin_cap) return;
    size_t new_cap = (lin_cap > 0) ? lin_cap * 2 : 64;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(lin_cache, new_cap * sizeof(seq_t));
    lin_cache = tmp;
    lin_cap   = new_cap;
}

/* Append a value to the linear cache. */
static void lin_push(seq_t val)
{
    ensure_lin_cap(lin_len + 1);
    lin_cache[lin_len++] = val;
}

/* Compute row r (0‑based) of the triangle and append its elements
 * to the linear cache.  Uses only the last three rows stored
 * in the circular buffer. */
static void compute_row(idx_t r)
{
    int idx = (int)(r % 4);
    row_len[idx] = r + 1;

    /* Base rows 0‑3. */
    if (r == 0) {
        rows[idx][0] = 1;
        lin_push(1);
        return;
    }
    if (r == 1) {
        rows[idx][0] = 1;
        rows[idx][1] = 1;
    } else if (r == 2) {
        rows[idx][0] = 1;
        rows[idx][1] = 2;
        rows[idx][2] = 1;
    } else if (r == 3) {
        rows[idx][0] = 1;
        rows[idx][1] = 4;
        rows[idx][2] = 2;
        rows[idx][3] = 1;
    } else {   /* r >= 4 */
        /* Fixed right‑hand side values */
        rows[idx][0]      = 1;
        rows[idx][r]      = 1;
        rows[idx][r - 1]  = 2;
        rows[idx][r - 2]  = 4;

        /* indices of the previous three rows */
        int idx_m3 = (int)((r - 3) & 3);
        int idx_m2 = (int)((r - 2) & 3);
        int idx_m1 = (int)((r - 1) & 3);

        /* Recurrence for columns 1 .. r-3 */
        for (int m = 1; m <= (int)r - 3; ++m) {
            rows[idx][m] =
                rows[idx_m3][m - 1] +
                rows[idx_m3][m] +
                rows[idx_m2][m] +
                rows[idx_m1][m];
        }
    }

    /* Append the full row to the linear cache. */
    for (idx_t k = 0; k <= r; ++k)
        lin_push(rows[idx][k]);
}

/* Public routine: return a(n). */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_row(0);      /* initialise with row 0 */
        initialized = true;
    }

    /* Generate rows until we have covered index n. */
    while (lin_len <= n) {
        compute_row(next_row++);
    }
    return lin_cache[n];
}
