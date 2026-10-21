/*
 * A080335  Diagonal in square spiral or maze arrangement of natural numbers.
 * https://oeis.org/A080335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 4*n + 2*n^2 - (-1)^n)/2
 * Range:     u8 n <= 10, u16 n <= 253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fn = (seq_t)n;
    seq_t term = (seq_t)3 + (seq_t)4 * fn + (seq_t)2 * fn * fn;
    if ((n & 1) == 0)          /* n even: (-1)^n = 1, subtract 1 */
        term -= (seq_t)1;
    else                       /* n odd:  (-1)^n = -1, subtract (-1) = +1 */
        term += (seq_t)1;
    return term / (seq_t)2;
}
