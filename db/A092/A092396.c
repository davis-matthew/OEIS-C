/*
 * A092396  Row 2 of array in A288580.
 * https://oeis.org/A092396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^⌊(n+1)/2⌋ * (n!!)^2 where n!! = product of positive
 *            integers ≤ n with the same parity as n (0!! is defined to be 1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;                          /* n!!  */
    idx_t start = (n % 2 == 0) ? 2 : 1;     /* start value for double factorial   */
    for (idx_t i = start; i <= n; i += 2) {
        prod *= (seq_t)i;
    }
    prod *= prod;                           /* square the double factorial          */
    /* sign factor: exponent is floor((n+1)/2); apply minus if it is odd */
    if (((n + 1) / 2) & 1u) {
        prod = -prod;
    }
    return prod;
}
