/*
 * A221833  Triangle, read by rows, where T(n,k) = [x^n] x^k*(1-x^k)^(k-1) /
 *          (1-x)^(k-1) for n>=k>=1.
 * https://oeis.org/A221833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient [x^n] x^k*(1-x^k)^(k-1)/(1-x)^(k-1) read
 *            row-wise
 * Range:     u8 n <= 50, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;                     /* current triangular number r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }
    idx_t k = n - (r - 1) * r / 2;       /* column index in row r */

    /* Special cases */
    if (k == 1) {
        return (r == 1) ? (seq_t)1 : (seq_t)0;
    }
    if (k == r) {
        return (seq_t)1;
    }

    /* General case */
    seq_t even_sum = 0;
    seq_t odd_sum  = 0;
    idx_t max_j = (r - k) / k;
    if (max_j > k - 1) max_j = k - 1;

    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t term = binom(k - 1, j);
        idx_t m = r - k - j * k;
        term *= binom(k - 2 + m, k - 2);
        if (j & 1)
            odd_sum += term;
        else
            even_sum += term;
    }
    return even_sum - odd_sum;           /* even_sum >= odd_sum for all cases */
}
