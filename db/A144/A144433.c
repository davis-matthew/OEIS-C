/*
 * A144433  Multiples of 8 interleaved with the sequence of odd numbers >= 3.
 * https://oeis.org/A144433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+1 for even n; a(n) = 4*(n+1) for odd n
 * Range:     u8 n <= 62, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n + 1;   /* n+1 */
    if (n & 1) {                   /* odd n */
        result *= 4;               /* 4*(n+1) */
    }
    return result;
}
