/*
 * A089249  Triangular array read by rows illustrating the connection between
 *          A000522 and A008292.
 * https://oeis.org/A089249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binom(r+1, k+1) * (2^(k+1) - k - 2), where r is the row
 *            index (first row =1) and k is the column index (first column =1)
 *            such that the triangular index T(r-1)+k = n. The diagonal values
 *            are d_k = 2^(k+1) - k - 2, which are the last entries of each
 *            prior row. binom(a,b) is the binomial coef...
 * Range:     u8 n <= 17, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below the offset is irrelevant */
        return 0;

    /* locate the row r such that T(r) = r*(r+1)/2 >= n,
       and the preceding triangular sum T(r-1) = sum - r. */
    idx_t row = 1;
    idx_t sum = 1;          /* T(1) */
    while (sum < n) {
        ++row;
        sum += row;         /* now sum == T(row) */
    }
    idx_t prev_sum = sum - row; /* T(row-1) */
    idx_t col = n - prev_sum;   /* 1 ≤ col ≤ row */

    /* diagonal value d_col = 2^(col+1) - col - 2 */
    seq_t power = pow2(col + 1);
    seq_t diag = power - ((seq_t)col + 2);

    /* combinatorial factor */
    seq_t bin = binom(row + 1, col + 1);

    return bin * diag;
}
