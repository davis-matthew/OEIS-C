/*
 * A205354  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          the same number of equal diagonal or antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A205354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 3*4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the offset */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    result = result * (seq_t)3 + (seq_t)2;
    return result;
}
