/*
 * A059587  T(n,m) = (1/m!)*Sum_{i=0..m}
 *          stirling1(m,i)*(2^i)*(2^i+1)*...*(2^i+n-1).
 * https://oeis.org/A059587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is obtained by flattening the triangle T(n,m) where n ≥ 0,
 *            0 ≤ m ≤ 2^n and T(n,m) = (1/m!) * Σ_{i=0..m} s1(m,i) *
 *            Π_{k=0}^{n-1} (2^i + k) Equivalently T(n,m) = Σ_{i=0..n}
 *            |S(n,i)| * binom(2^i, m) where S(n,i) is the (signless) Stirling
 *            number of the first kind and binom(a,b) is the binomial...
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

#define MAX_R 64   /* enough for any n ≤ 10⁶ (2^20 ≈ 10⁶) */

static seq_t stirling[MAX_R + 1][MAX_R + 1];
static int   stirling_max = -1;

/* Build Stirling numbers of first kind up to n (absolute values).   */
static void compute_stirling(int n)
{
    if (stirling_max >= n) return;

    if (stirling_max < 0) {         /* initialise base case */
        stirling[0][0] = 1;
        stirling_max   = 0;
    }

    for (int row = stirling_max + 1; row <= n; ++row) {
        stirling[row][0] = 0;
        for (int col = 1; col <= row; ++col) {
            /* S(row,col) = S(row-1,col-1) + (row-1)*S(row-1,col) */
            stirling[row][col] = stirling[row-1][col-1]
                               + (seq_t)(row-1) * stirling[row-1][col];
        }
    }
    stirling_max = n;
}

/* Convert a global index n into (row r, column m) of the triangle. */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t cumulative = 0;
    for (int r = 0;; ++r) {
        idx_t row_len = ((idx_t)1 << r) + 1;       /* 2^r + 1 */
        if (n < cumulative + row_len) {
            *row = r;
            *col = n - cumulative;
            return;
        }
        cumulative += row_len;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r and column m that contain the nth entry. */
    idx_t r, m;
    index_to_row_col(n, &r, &m);

    /* Compute Stirling numbers up to row r. */
    compute_stirling((int)r);

    /* Use the alternative formula:  T(r,m) = Σ_{i=0..r} |S(r,i)| * binom(2^i, m) */
    seq_t sum = 0;
    for (int i = 0; i <= r; ++i) {
        seq_t s = stirling[(int)r][i];          /* |S(r,i)| is already unsigned */
        /* binom returns 0 when m > 2^i, so no special test needed */
        seq_t b = binom(((idx_t)1 << i), m);
        sum += s * b;
    }
    return sum;
}
