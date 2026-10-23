/*
 * A168670  Numbers that are congruent to {1, 8} mod 11.
 * https://oeis.org/A168670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*floor(n/2) + (n%2 ? 1 : -3)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n<1, return 0 */

    seq_t half = (seq_t)(n / 2);    /* floor(n/2)  */

    if ((n & 1) == 0)              /* n even  */
        return 11 * half - 3;
    else                            /* n odd   */
        return 11 * half + 1;
}
