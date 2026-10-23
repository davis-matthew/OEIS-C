/*
 * A158607  Period 5: repeat [9, 11, 13, 5, 7].
 * https://oeis.org/A158607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 7 - 10*floor((n+1)/5) (offset = 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n>=1 for valid terms.  For n==0
       we return 0, though this case is not used. */
    if (n == 0) return 0;

    /* Convert the index to seq_t before arithmetic to avoid
       signed/unsigned mixing and to keep intermediate results
       within the width of seq_t. */
    seq_t nn = (seq_t)n;

    /* Compute a(n) according to the formula.  Integer division
       truncates towards zero, which is equivalent to floor
       for unsigned values. */
    seq_t result = 2 * nn + 7 - 10 * ((nn + 1) / 5);

    return result;
}
