/*
 * A101409  Triangle read by rows: T(n,k) is the number of noncrossing trees
 *          with n edges in which the leftmost leaf is at level k.
 * https://oeis.org/A101409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = ceil((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r-1)/2, and T(r,c) = Σ_{i=0}^{c-1} ((c+i)/(2*r-c+i)) *
 *            binom(c-1,i) * binom(3*r-2*c+i-1, r-c)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 120, u64 n <= 378,
 *            u128 n <= 1128, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t triangle_term(idx_t row, idx_t col)
{
    seq_t sum = 0;
    idx_t nmk = row - col;               /* n - k  */
    for (idx_t i = 0; i < col; ++i) {
        seq_t bin1 = binom(col - 1, i);
        idx_t arg2 = 3 * row - 2 * col + i - 1;
        seq_t bin2 = binom(arg2, nmk);
        unsigned __int128 prod =
            (unsigned __int128)(col + i) *
            (unsigned __int128)bin1 *
            (unsigned __int128)bin2;
        seq_t denom = (2 * row - col + i);
        seq_t term = (seq_t)(prod / denom);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, so n≥1 in practice */
    /* find row r so that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev = r * (r - 1) / 2;
    idx_t c = n - prev;                 /* column index */
    return triangle_term(r, c);
}
