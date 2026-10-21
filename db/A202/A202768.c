/*
 * A202768  Vandermonde determinant of the first n squares.
 * https://oeis.org/A202768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (2*k+1)!/(k+1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = 1;          /* current a(k) value               */
    seq_t fact_odd = 1;       /* (2*k+1)!  for current k          */

    for (idx_t k = 0; k < n; ++k) {
        a_val = a_val * fact_odd / (k + 1);               /* a(k+1) = a(k) * (2k+1)!/(k+1) */
        fact_odd = fact_odd * (2 * (k + 1)) * (2 * (k + 1) + 1); /* next odd factorial */
    }

    return a_val;
}
