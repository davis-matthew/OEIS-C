/*
 * A205187  Number of (n+1)X2 0..1 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock differing from each horizontal or
 *          vertical neighbor.
 * https://oeis.org/A205187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * a(n-2) Initial values: a(1) = 16, a(2) = 24
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 76, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)24;

    seq_t res = (n % 2 == 1) ? (seq_t)16 : (seq_t)24;
    while (n > 2) {
        res *= (seq_t)3;
        n -= 2;
    }
    return res;
}
