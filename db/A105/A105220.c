/*
 * A105220  Trajectory of 1 under the morphism 1->{1,2,1}, 2->{2,2,2}.
 * https://oeis.org/A105220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if the ternary expansion of n contains the digit 1,
 *            otherwise a(n) = 1
 * Range:     u8 n <= 9994, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    while (n > 0) {
        /* Check the least significant ternary digit */
        if (n % 3 == 1)
            return (seq_t)2;
        n /= 3;
    }
    /* n == 0 has no digit 1 in its ternary representation */
    return (seq_t)1;
}
