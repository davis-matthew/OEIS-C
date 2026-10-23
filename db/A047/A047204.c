/*
 * A047204  Numbers that are congruent to {3, 4} mod 5.
 * https://oeis.org/A047204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n/2) - (-1)^n
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(5*n/2) using integer arithmetic */
    seq_t tmp = (seq_t)n * (seq_t)5;
    seq_t val = tmp >> 1;          /* truncating division by 2 */

    /* Subtract (-1)^n: add 1 if n is odd, subtract 1 if n is even */
    if ((n & 1) == 0)              /* even n -> (-1)^n = +1 */
        val -= 1;
    else                           /* odd  n -> (-1)^n = -1 */
        val += 1;

    return val;
}
