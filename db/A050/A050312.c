/*
 * A050312  Matrix 9th power of inverse partition triangle A038498.
 * https://oeis.org/A050312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = entry of the 9th power of the inverse partition triangle
 *            A038498, flattened row‑wise
 * Range:     i8 n <= 15, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_ROWS 200   /* sufficient for n up to ≈20 000 */

static bool initialized = false;
static seq_t partition[MAX_ROWS + 1][MAX_ROWS + 1];
static seq_t inv[MAX_ROWS + 1][MAX_ROWS + 1];
static seq_t M9[MAX_ROWS + 1][MAX_ROWS + 1];

/* matrix multiplication: C = A * B (size d × d) */
static void mat_mul(const seq_t A[][MAX_ROWS + 1], const seq_t B[][MAX_ROWS + 1],
                    seq_t C[][MAX_ROWS + 1], idx_t d)
{
    for (idx_t i = 1; i <= d; ++i) {
        for (idx_t j = 1; j <= d; ++j) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= d; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* precompute partition numbers, their inverse, and the 9th power */
static void init(void)
{
    /* partition numbers p[n][k] for 1 ≤ k ≤ n */
    for (idx_t n = 1; n <= MAX_ROWS; ++n) {
        partition[n][1] = 1;
        partition[n][n] = 1;
        for (idx_t k = 2; k <= n; ++k) {
            if (k == n) continue;
            partition[n][k] = partition[n - 1][k - 1];
            if (n > k && n - k >= k) {
                partition[n][k] += partition[n - k][k];
            }
        }
    }

    /* inverse partition triangle inv = A038498 */
    for (idx_t i = 1; i <= MAX_ROWS; ++i) {
        inv[i][i] = 1;
        for (idx_t j = 1; j < i; ++j) {
            seq_t sum = 0;
            for (idx_t k = j; k < i; ++k) {
                sum += partition[i][k] * inv[k][j];
            }
            inv[i][j] = -sum;
        }
    }

    /* compute inv^9 */
    seq_t res[MAX_ROWS + 1][MAX_ROWS + 1];
    seq_t temp[MAX_ROWS + 1][MAX_ROWS + 1];

    /* identity matrix */
    for (idx_t i = 1; i <= MAX_ROWS; ++i) {
        for (idx_t j = 1; j <= MAX_ROWS; ++j) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }

    for (int m = 0; m < 9; ++m) {
        mat_mul(res, inv, temp, MAX_ROWS);
        for (idx_t i = 1; i <= MAX_ROWS; ++i) {
            for (idx_t j = 1; j <= MAX_ROWS; ++j) {
                res[i][j] = temp[i][j];
            }
        }
    }

    for (idx_t i = 1; i <= MAX_ROWS; ++i) {
        for (idx_t j = 1; j <= MAX_ROWS; ++j) {
            M9[i][j] = res[i][j];
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    /* find row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    idx_t prev = (r - 1) * r / 2; /* number of entries before row r */
    idx_t c = n - prev;           /* column index (1‑based) */

    return M9[r][c];
}
