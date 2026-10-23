/*
 * A168577  Pascal's triangle, first two columns and diagonal removed.
 * https://oeis.org/A168577
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r, k) where r = floor((sqrt(8*(n-2)+1)-1)/2)+3
 *            and k = 2 + (n - (2 + (r-3)*(r-2)/2)) (i.e. r is the row index
 *            and k is the column index of the entry after removing the first
 *            two columns and the diagonal)
 * Range:     u8 n <= 18, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For n < 2 the return value
       is undefined; we return 0 for safety. */
    if (n < 2) return 0;

    /* Compute t = floor( (-1 + sqrt(1+8*(n-2)) ) / 2 ).  We use the
       available integer square‑root routine. */
    idx_t diff = n - 2;                         /* n-2 >= 0 */
    idx_t sqrt_term = _floor_isqrt_u64(1 + 8 * diff);
    idx_t t = (sqrt_term - 1) / 2;

    /* r = t + 3 is the row index (starting at 3). */
    idx_t r = t + 3;

    /* Starting index of the row in the sequence. */
    idx_t start = 2 + (r - 3) * (r - 2) / 2;

    /* Position within the row (0‑based). */
    idx_t pos = n - start;

    /* Column index within the row. */
    idx_t k = 2 + pos;

    /* Return the binomial coefficient.  The helper binomial() performs
       the calculation multiplicatively and returns a value of type
       seq_t, which meets the required type precision. */
    return binomial(r, k);
}
