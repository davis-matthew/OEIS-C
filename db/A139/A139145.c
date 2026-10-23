/*
 * A139145  a(1) = 1, a(2*n) = a(n)^2, a(2*n+1) = a(n)*(a(n)+1).
 * https://oeis.org/A139145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2n) = a(n)^2, a(2n+1) = a(n)*(a(n)+1)
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if ((n & 1ULL) == 0) {               /* even: 2*n */
        idx_t half = n >> 1;
        seq_t x = a(half);
        return x * x;
    } else {                               /* odd: 2*n+1 */
        idx_t half = n >> 1;
        seq_t x = a(half);
        return x * (x + 1);
    }
}
