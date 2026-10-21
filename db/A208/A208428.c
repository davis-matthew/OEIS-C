/*
 * A208428  Number of n X 2 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to any knight-move neighbor
 *          (colorings ignoring permutations of colors).
 * https://oeis.org/A208428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*2^(2*n-5) for n>2, with a(1)=2, a(2)=14, a(3)=54
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalently, a(4)=216 and a(n)=4*a(n-1) for n>3. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)54;

    /* For n >= 4, a(n) = 54 * 4^(n-3) = 27 * 2^(2*n-5) */
    seq_t res = (seq_t)54;
    for (idx_t i = 4; i <= n; ++i) {
        res *= (seq_t)4;
    }
    return res;
}
