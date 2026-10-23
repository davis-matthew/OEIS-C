/*
 * A143261  Triangle read by rows: binary reversed Gray code of binomial(n,m).
 * https://oeis.org/A143261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = reverse_binary( Gray( binomial(r, j) ) ), where r =
 *            floor((sqrt(8*n+1)-1)/2), j = n - r*(r+1)/2. Gray(x) = x ^ (x >>
 *            1)
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t reverse_bits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in row r. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* j is the column index in that row. */
    idx_t j = n - r * (r + 1) / 2;
    seq_t bin = binomial(r, j);
    seq_t gray = bin ^ (bin >> 1);
    return reverse_bits(gray);
}
