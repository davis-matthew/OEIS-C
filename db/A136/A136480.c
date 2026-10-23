/*
 * A136480  Number of trailing equal digits in binary representation of n.
 * https://oeis.org/A136480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of trailing equal digits in the binary
 *            representation of n a(0) = 1 if n > 0: a(n) = number of trailing
 *            binary zeros when n is even, a(n) = number of trailing binary
 *            ones when n is odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* special case for n = 0 */
        return 1;

    if ((n & 1ULL) == 0) {           /* n is even – count trailing zeros */
        seq_t count = 0;
        while ((n & 1ULL) == 0) {
            count++;
            n >>= 1;
        }
        return count;
    } else {                         /* n is odd – count trailing ones */
        seq_t count = 0;
        while ((n & 1ULL) == 1) {
            count++;
            n >>= 1;
        }
        return count;
    }
}
