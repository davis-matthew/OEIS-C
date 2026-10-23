/*
 * A126347  Triangle, read by rows, where row n lists coefficients of q in
 *          B(n,q) that satisfies: B(n,q) = Sum_{k=0..n-1} C(n-1,k)*B(k,q)*q^k
 *          for n>0, with B(0,q) = 1; row sums equal the Bell numbers: B(n,1)
 *          = A000110(n).
 * https://oeis.org/A126347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of q^k in B(r,q) where B(0,q) = 1; for
 *            n>0, B(n,q) = ∑_{k=0}^{n-1} C(n-1,k) * B(k,q) * q^k The triangle
 *            is read by rows; row r has r*(r-1)/2+1 coefficients. The
 *            sequence a(n) is obtained by concatenating the rows
 * Range:     u8 n <= 70, u16 n <= 244, u32 n <= 863, u64 n <= 3379,
 *            u128 n <= 9347, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

#define MAX_ROWS 256

static seq_t *rows[MAX_ROWS];
static idx_t row_start[MAX_ROWS];
static idx_t row_count = 0;

/* binomial coefficient C(n,k) for 0<=k<=n */
static inline seq_t binom_nk(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* row length: number of coefficients in row r */
static inline idx_t row_len(idx_t r)
{
    return r * (r - 1) / 2 + 1;
}

/* ensure rows up to cover index n */
static void ensure_rows(idx_t n)
{
    if (row_count == 0) {
        /* compute row 0 */
        rows[0] = alloc_seq_array((size_t)row_len(0));
        rows[0][0] = 1;
        row_start[0] = 0;
        row_count = 1;
    }

    while (1) {
        idx_t last_start = row_start[row_count - 1];
        idx_t len_last   = row_len(row_count - 1);
        idx_t next_start = last_start + len_last;           /* start of next row */
        if (n < next_start) break;                         /* already covered */

        /* compute row r = row_count */
        idx_t r      = row_count;
        idx_t len_r  = row_len(r);
        seq_t *row   = alloc_seq_array((size_t)len_r);
        /* initialize to zero (alloc_seq_array does it) */
        for (idx_t k = 0; k < r; ++k) {
            seq_t bin = binom_nk(r - 1, k);
            seq_t *Bk = rows[k];
            idx_t lenBk = row_len(k);
            for (idx_t e = 0; e < lenBk; ++e) {
                row[e + k] += bin * Bk[e];
            }
        }
        rows[r] = row;
        row_start[r] = next_start;   /* start index of this row */
        row_count++;

        if (row_count >= MAX_ROWS) break;  /* safety, should not happen for n≤10000 */
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_rows(n);

    /* locate the row containing n */
    idx_t row = 0;
    while (row + 1 < row_count && n >= row_start[row + 1]) row++;

    idx_t offset = n - row_start[row];
    return rows[row][(size_t)offset];
}
