/*
 * A104566  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product H*R of the infinite lower triangular matrices H = [1;
 *          1,2; 1,2,1; 1 2,1,2; ...] and R = [1; 1,1; 1,1,1; 1,1,1,1; ...].
 * https://oeis.org/A104566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row_of(n), column_of(n)) where T(i,j) = { 3*(i-j+1)/2
 *            if i and j have opposite parity, 3*(i-j)/2 + 1 if i and j are
 *            both odd, 3*(i-j)/2 + 2 if i and j are both even
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row i such that i*(i+1)/2 >= n */
    idx_t lo = 1, hi = n; /* row cannot exceed n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t i = lo;

    /* Compute column j */
    __uint128_t prev_tri = (__uint128_t)(i - 1) * i / 2;
    idx_t j = n - (idx_t)prev_tri;

    /* Compute T(i,j) */
    idx_t diff = i - j;
    seq_t result;
    if ((i ^ j) & 1U) {               /* opposite parity */
        result = (seq_t)3 * (seq_t)((diff + 1) / 2);
    } else if ((i & 1U) && (j & 1U)) { /* both odd */
        result = (seq_t)3 * (seq_t)(diff / 2) + 1;
    } else {                           /* both even */
        result = (seq_t)3 * (seq_t)(diff / 2) + 2;
    }
    return result;
}
