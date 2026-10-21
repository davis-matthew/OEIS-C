/*
 * A215495  a(4*n) = a(4*n+2) = a(2*n+1) = 2*n + 1.
 * https://oeis.org/A215495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*n) = a(4*n+2) = a(2*n+1) = 2*n + 1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Odd indices: a(n) = n. */
    if ((n & 1) != 0) {
        return (seq_t)n;
    }

    /* Even indices: a(n) = (n/2) + (n % 4 == 0 ? 1 : 0). */
    seq_t k = (seq_t)(n >> 1);   /* k = n/2 */
    if ((n & 3) == 0) {          /* n % 4 == 0 */
        return k + 1;
    }
    return k;
}
