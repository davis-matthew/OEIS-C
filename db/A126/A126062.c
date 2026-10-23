/*
 * A126062  Array read by antidiagonals: see A128195 for details.
 * https://oeis.org/A126062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(k,n_col) where * s = floor((sqrt(1+8 n)-1)/2) (diagonal
 *            index) * d = n – s(s+1)/2 (offset inside diagonal) * k = s – d
 *            (row index) * n_col = d (column index) * * Recurrence for
 *            T(k,m): T(k,0) = 1, * T(k,m) = (k*m+1) * (T(k,m-1) + k^m)
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, not required but harmless */

/* Compute the entry T(k,m) of the array. */
static seq_t compute_T(seq_t k, idx_t m)
{
    seq_t result = 1;   /* T(k,0) */
    seq_t power  = 1;   /* k^0 */

    for (idx_t i = 1; i <= m; ++i) {
        power *= k;               /* power = k^i */
        seq_t coeff = ((seq_t)i * k) + 1;   /* (i*k + 1) */
        result = coeff * (result + power);
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    idx_t triangular = s * (s + 1) / 2;   /* first index of diagonal s */
    idx_t d          = n - triangular;   /* offset within diagonal */
    seq_t k          = (seq_t)(s - d);    /* row index */
    idx_t col        = d;                /* column index */

    return compute_T(k, col);
}
