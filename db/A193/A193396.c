/*
 * A193396  Hyper-Wiener index of a benzenoid consisting of a double-step
 *          zig-zag chain of n hexagons (n >= 2, s = 2123; see the Gutman et
 *          al. reference).
 * https://oeis.org/A193396
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^4 + 24*n^3 + 28*n^2 + 213*n + 33*(-1)^n - 246)/3
 * Range:     u8 none, u16 n <= 8, u32 n <= 198, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;        /* n^2 */
    seq_t x3 = x2 * x;       /* n^3 */
    seq_t x4 = x3 * x;       /* n^4 */

    /* build the polynomial part */
    seq_t sum = (seq_t)8 * x4 + (seq_t)24 * x3 + (seq_t)28 * x2 + (seq_t)213 * x;

    /* add 33*(-1)^n : add 33 if n even, subtract 33 if n odd */
    if ((n & 1) == 0)
        sum += (seq_t)33;
    else
        sum -= (seq_t)33;

    /* final corrections and division by 3 */
    sum -= (seq_t)246;
    return sum / (seq_t)3;
}
