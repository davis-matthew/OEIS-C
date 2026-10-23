/*
 * A011973  Irregular triangle read by rows: T(n,k) = binomial(n-k, k), n >=
 *          0, 0 <= k <= floor(n/2); or, coefficients of (one version of)
 *          Fibonacci polynomials.
 * https://oeis.org/A011973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, nonn, nice
 * Formula:   a(n) is the entry of the irregular triangle T(r,k)=C(r-k,k) *
 *            read by rows. The global index n is first the element of row 0,
 *            * then row 1, then row 2, etc. For a given n find the row r such
 *            that * the cumulative number of entries before r is ≤ n <
 *            cumulative up to r, * then compute k = n – cumul...
 * Range:     u8 n <= 67, u16 n <= 202, u32 n <= 688, u64 n <= 2474,
 *            u128 n <= 2600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Multiplicative binomial coefficient that never overflows the
 * intermediate result when the final value fits into seq_t.
 * For small arguments (n≤200 in the typical use of A011973) the
 * product (result*term) fits into unsigned __int128, used only
 * as a temporary. */
static seq_t my_binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        unsigned __int128 tmp = (unsigned __int128)result * (n - k + i);
        result = (seq_t)(tmp / i);      /* division is exact */
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t r = 0;           /* current row number */
    idx_t prefix = 0;      /* cumulative entries before row r */

    while (1)
    {
        idx_t count = (r >> 1) + 1;        /* entries in row r */
        if (prefix + count > n) {          /* n lies in this row */
            idx_t col = n - prefix;       /* column index k in row r */
            /* T(r,k) = C(r-k, k) */
            return my_binomial(r - col, col);
        }
        prefix += count;
        ++r;
    }
}
