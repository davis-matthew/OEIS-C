/*
 * A203572  Period length 12: 0, 1, 2, 3, 4, 5, 0, 5, 4, 3, 2, 1 repeated.
 * https://oeis.org/A203572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 6 if floor(n/6) is even * a(n) = (6 - (n mod 6))
 *            mod 6 if floor(n/6) is odd * Period = 12:
 *            0,1,2,3,4,5,0,5,4,3,2,1 repeated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 12 */
    static const seq_t pattern[12] = {
        0, 1, 2, 3, 4, 5,
        0, 5, 4, 3, 2, 1
    };

    /* Take remainder of n divided by 12 to index the pattern */
    return pattern[n % 12];
}
