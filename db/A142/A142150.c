/*
 * A142150  The nonnegative integers interleaved with 0's.
 * https://oeis.org/A142150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is even) ? n/2 : 0
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                a(2*n) = n, a(2*n+1) = 0 */

seq_t a(idx_t n)
{
    /* If n is odd, the term is 0 */
    if ((n & 1ULL) != 0) {
        return (seq_t)0;
    }
    /* For even n, return n/2, casted to seq_t */
    return (seq_t)(n >> 1);
}
