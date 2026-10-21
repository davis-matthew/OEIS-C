/*
 * A220000  Sixty fourths of an inch in thousandths, rounded to nearest
 *          integer.
 * https://oeis.org/A220000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(125*n/8) (0.5 rounded up)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 in the OEIS domain.
       For n == 0 the formula gives 0, which is harmless. */
    seq_t nn = (seq_t)n;
    /* Compute (125*n + 4)/8 using integer arithmetic.
       125 and 4 are small enough to avoid overflow when cast to seq_t. */
    return ((nn * (seq_t)125) + (seq_t)4) / (seq_t)8;
}
