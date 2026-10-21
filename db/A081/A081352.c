/*
 * A081352  Main diagonal of square maze arrangement of natural numbers
 *          A081349.
 * https://oeis.org/A081352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2) - (-1)^n
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = ((seq_t)n + 1) * ((seq_t)n + 2);
    if ((n & 1) == 0)           /* even n: subtract 1 */
        return prod - 1;
    else                         /* odd n: add 1 */
        return prod + 1;
}
