/*
 * A110124  A scaled Legendre triangle.
 * https://oeis.org/A110124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,c), where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r*(r+1)/2, and T(r,c) = Σ_{j=0}^{⌊(r-c)/2⌋} (-1)^j * C(r-c, j) *
 *            C(2r-2c-2j, r-c) * c^(r-c-2j)
 * Range:     i8 n <= 16, i16 n <= 37, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_small.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;
    idx_t diff = r - c;

    seq_t sum = 0;
    for (idx_t j = 0; j <= diff / 2; ++j) {
        seq_t term = binom_small(diff, j);
        term *= binom_small(2 * r - 2 * c - 2 * j, diff);
        seq_t pw = pow_seq((seq_t)c, diff - 2 * j);
        term *= pw;
        if (j & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
