/*
 * A168188  a(n) = n^3*(n^7 + 1)/2.
 * https://oeis.org/A168188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 * (n^7 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t n3 = x * x * x;           /* n^3 */
    seq_t n7 = n3 * n3 * x;          /* n^7 = (n^3)^2 * n */

    seq_t result;
    if ((x & 1) == 0) {             /* n even: divide n^3 by 2 first */
        result = (n3 / 2) * (n7 + 1);
    } else {                        /* n odd: divide (n^7 + 1) by 2 first */
        result = n3 * ((n7 + 1) / 2);
    }
    return result;
}
