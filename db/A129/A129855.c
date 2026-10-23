/*
 * A129855  A symmetrical triangle of coefficients based on A000217: a(n) =
 *          Binomial[n + 2, 2]; t(n,m)=a(n - m + 1)*a(m + 1).
 * https://oeis.org/A129855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Binomial[(r-c+1)+2,2] * Binomial[(c+1)+2,2] where (r,c)
 *            are the row and column (starting at 0) such that the flattened
 *            index n corresponds to the element t(r,c) = a(r-c+1)*a(c+1). Row
 *            0 contains the single entry 9
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binom_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_a.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence defined only for n >= 1 */

    idx_t z = n - 1;               /* zero–based index of the element */
    idx_t r = 0;

    /* find the row r such that z < (r+1)(r+2)/2  */
    while ((r + 1) * (r + 2) / 2 <= z)
        ++r;

    idx_t prev = r * (r + 1) / 2;  /* number of elements before row r */
    idx_t c = z - prev;            /* column within the row (0 .. r) */

    seq_t val1 = binom_a(r - c + 1);
    seq_t val2 = binom_a(c + 1);
    return val1 * val2;
}
