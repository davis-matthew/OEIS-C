/*
 * A181878  Coefficient array for square of Chebyshev S-polynomials.
 * https://oeis.org/A181878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 75, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A181878: Coefficient array for square of Chebyshev S-polynomials.
   Flattened from the triangle a(r,m) given by
     a(2k,m)   = (-1)^m Σ_{j=0..k} binomial(2k+m-1-2j, 2m-1)
     a(2k+1,m) = (-1)^m Σ_{j=0..k} binomial(2k+1+m-2j, 2m+1)
   with 0 ≤ m ≤ 2k, and the triangle is read row‑by‑row.
   Starting indices: a(0)=1, a(1)=1, … */

#include "../utils/binom.h"

/* Given the global index n, compute the triangle row r and column m. */
static void row_col(idx_t n, idx_t *r, idx_t *c)
{
    idx_t row = 0;
    idx_t cum = 0;                    /* cumulative length of rows 0 .. row‑1 */
    while (1) {
        idx_t k   = row >> 1;          /* floor(row/2) */
        idx_t len = (k << 1) + 1;      /* length of this row */
        if (cum + len > n) {
            *r = row;
            *c = n - cum;
            return;
        }
        cum += len;
        ++row;
    }
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    idx_t r, m;
    row_col(n, &r, &m);
    idx_t k = r >> 1;                  /* k = floor(r/2) */
    seq_t sum = 0;

    if ((r & 1) == 0) {                /* even row 2k */
        if (m == 0) {                  /* a(2k,0) is always 1 */
            sum = 1;
        } else {
            idx_t low = (m << 1) - 1;           /* 2m - 1  (≥ 1) */
            idx_t up0 = (k << 1) + m - 1;       /* 2k + m - 1 */

            for (idx_t j = 0; j <= k; ++j) {
                idx_t offset = j << 1;           /* 2*j */
                if (up0 < offset) break;         /* further j gives underflow */
                idx_t up = up0 - offset;         /* current upper index */

                if (up < low) continue;          /* binomial = 0 */
                sum += binom(up, low);
            }
        }
    } else {                           /* odd row 2k+1 */
        idx_t low = (m << 1) + 1;           /* 2m + 1 */
        idx_t up0 = (k << 1) + 1 + m;       /* 2k + 1 + m */

        for (idx_t j = 0; j <= k; ++j) {
            idx_t offset = j << 1;           /* 2*j */
            if (up0 < offset) break;         /* further j gives underflow */
            idx_t up = up0 - offset;         /* current upper index */

            if (up < low) continue;          /* binomial = 0 */
            sum += binom(up, low);
        }
    }

    /* apply (-1)^m */
    if (m & 1) {
        return -sum;
    } else {
        return sum;
    }
}
