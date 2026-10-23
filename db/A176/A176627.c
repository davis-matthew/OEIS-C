/*
 * A176627  Triangle T(n, k) = 12^(k*(n-k)), read by rows.
 * https://oeis.org/A176627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 12^(k*(n‑k)) read by rows of the triangle
 *            T(r,k)=12^(k*(r‑k)), where row r contains r+1 elements
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 23, u64 n <= 47, u128 n <= 83,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Find the row r such that the first index of row r is
       the smallest number > n in the cumulative count.  We
       accumulate the size of each row until the cumulative
       count surpasses n.  Row 0 has 1 element, row 1 has 2,
       etc.                                    */
    idx_t r = 0;
    idx_t cum = 1;   /* cumulative count after row 0 */
    while (cum <= n) {
        r++;
        cum += r + 1;   /* add the size of the next row (r+1) */
    }

    /* The number of elements before the start of row r */
    idx_t prev = cum - (r + 1);

    /* Column index k within this row (0‑based) */
    idx_t k = n - prev;

    /* Exponent e = k*(r‑k).  Both operands fit in idx_t,
       and the product fits comfortably in idx_t for n <= 10000. */
    idx_t e = k * (r - k);

    /* Compute 12^e.  big_pow handles arbitrary‑precision values
       correctly for any seq_t. */
    return big_pow((seq_t)12, (seq_t)e);
}
