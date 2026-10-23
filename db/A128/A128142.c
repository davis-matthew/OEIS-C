/*
 * A128142  A128132 * A122432 (unsigned).
 * https://oeis.org/A128142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128142: product of the lower triangular arrays A128132 and A122432.
   For n ≥ 1 (offset 1) a(n) is the entry found by walking through the infinite
   matrix row‑wise, i.e. a(1)=C(1,1), a(2)=C(2,1), a(3)=C(2,2), a(4)=C(3,1), … .
   The entry C(i,j) (1 ≤ j ≤ i) of the product can be computed by the closed
   formula

          C(i,j) = ((i-1)·j² + i·(i+1)² - (2·i² + i - 1)·j) / 2

   where i is the row index and j the column index.  For a given n we find
   i by solving i·(i+1)/2 ≥ n and then set j = n – i·(i-1)/2.  The result
   fits in seq_t for all n that fit in the underlying integer type, and the
   computation uses only integer arithmetic.
   The implementation runs in O(1) time per call and works for the built‑in
   unsigned integer types as well as arbitrary‑precision types.  */

#include <math.h>

seq_t a(idx_t n)
{
    /* Determine the row i such that i*(i+1)/2 ≥ n. */
    long double t = sqrt((long double)8 * (long double)n + 1.0L);
    idx_t i = (idx_t)((t - 1.0L) / 2.0L);

    /* Correct possible rounding errors. */
    while ((seq_t)i * (i + 1) / 2 < n) ++i;
    while (i > 0 && (seq_t)(i - 1) * i / 2 >= n) --i;

    /* Column index within the row (1‑based). */
    idx_t j = n - (idx_t)((i - 1) * i / 2);

    /* Convert to seq_t for arithmetic. */
    seq_t i_s = (seq_t)i;
    seq_t j_s = (seq_t)j;

    /* Compute the numerator of the formula. */
    seq_t sum1 = (i_s - 1) * j_s * j_s;                      /* (i-1)·j²          */
    seq_t sum2 = i_s * (i_s + 1) * (i_s + 1);                 /* i·(i+1)²          */
    seq_t sum  = sum1 + sum2;                                /* sum of the two terms */

    seq_t sub   = ((seq_t)2 * i_s * i_s + i_s - 1) * j_s;    /* (2·i² + i – 1)·j   */

    /* Final result.  The numerator is always even. */
    return (sum - sub) / 2;
}
