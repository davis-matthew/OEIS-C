/*
 * A168623  Triangle read by rows: T(n, k) = [x^k]( 9*(1+x)^n - 8*(1 + x^n) ),
 *          with T(0, 0) = 1.
 * https://oeis.org/A168623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,c) * ( (c==0||c==r)?1:9 ), where r and c are
 *            the row and column of the flat index n in the triangle read by
 *            rows
 * Range:     u8 n <= 30, u16 n <= 141, u32 n <= 541, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r and the column c that correspond to the flat
       index n.  Row r has (r+1) entries, so we cumulatively add
       row sizes until the next total would exceed n. */
    idx_t r = 0;
    idx_t cumulative = 0;

    while (cumulative + (r + 1) <= n) {
        cumulative += (r + 1);
        ++r;
    }
    idx_t c = n - cumulative;

    /* Edge entries equal 1; interior entries are 9 times the binomial. */
    if (c == 0 || c == r) {
        return (seq_t)1;
    } else {
        seq_t b = binomial(r, c);
        return b * (seq_t)9;
    }
}
