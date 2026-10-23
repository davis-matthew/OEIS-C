/*
 * A182411  Triangle T(n,k) = (2*k)!*(2*n)!/(k!*n!*(k+n)!) with k=0..n, read
 *          by rows.
 * https://oeis.org/A182411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look, easy
 * Formula:   a(n) is the n-th element of the triangle T(n,k) =
 *            (2*k)!*(2*n)!/(k!*n!*(k+n)!) (k = 0..n), read by rows. The
 *            function computes T(row, col) for the given index n
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Binomial coefficient C(2n,n) computed multiplicatively. */
static inline seq_t binom_2n_n(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)(n + i);      /* (n+i) fits in idx_t */
        res = res * num / (seq_t)i;      /* exact division */
    }
    return res;
}

/* T(row,k) computed from T(row,0)=C(2row,row) using the recurrence */
static inline seq_t T_rc(idx_t row, idx_t k)
{
    seq_t res = binom_2n_n(row);           /* base value T(row,0) */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)2 * (seq_t)(2 * i - 1);  /* 2*(2i-1) */
        seq_t den = (seq_t)(row + i);               /* row+i */
        res = res * num / den;      /* exact division */
    }
    return res;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    /* Determine which row (row) the index n falls into.
       The condition is: row*(row+1)/2 <= n < (row+1)*(row+2)/2  */
    idx_t row = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);
    /* Correct possible round‑off errors from the square root. */
    while ((row + 1) * (row + 2) / 2 <= n) ++row;
    while (row * (row + 1) / 2 > n) --row;

    /* Column index within the row */
    idx_t col = n - row * (row + 1) / 2;

    return T_rc(row, col);
}
