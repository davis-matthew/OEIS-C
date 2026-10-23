/*
 * A183109  Triangle read by rows: T(n,m) = number of n X m binary matrices
 *          with no zero rows or columns (n >= 1, 1 <= m <= n).
 * https://oeis.org/A183109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(row, col) where T(k, m) = Σ_{j=0..m} (-1)^j * C(m, j) *
 *            (2^(m-j) - 1)^k . The triangle is read by rows, rows starting at
 *            k = 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 20, u64 n <= 43, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "..//utils/binom.h"
#include "..//utils/pow2.h"
#include "..//utils/seq_pow.h"

/* Find the row r such that r*(r+1)/2 ≥ n */
static idx_t find_row(idx_t n)
{
    idx_t r = 1;
    /* Use unsigned 128‑bit arithmetic for safety when n is large. */
    while ( ((unsigned __int128)r * (r + 1)) / 2 < n )
        ++r;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine (row, col) in the triangle */
    idx_t row = find_row(n);
    idx_t prev = ((unsigned __int128)(row - 1) * row) / 2;   /* terms before this row */
    idx_t col  = n - prev;                                   /* 1 ≤ col ≤ row */

    /* Compute the inclusion–exclusion sum for T(row, col) */
    seq_t pos = 0;   /* sum of terms with even j */
    seq_t neg = 0;   /* sum of terms with odd  j */

    for (idx_t j = 0; j <= col; ++j) {
        seq_t bin   = binom(col, j);                          /* C(col, j) */
        seq_t base  = pow2(col - j) - (seq_t)1;              /* 2^(col-j) – 1 */
        seq_t power = seq_pow(base, row);                    /* (2^(col-j)-1)^row */

        seq_t term   = bin * power;                          /* term value */

        if (j & 1)
            neg += term;
        else
            pos += term;
    }

    return pos - neg;   /* final result */
}
