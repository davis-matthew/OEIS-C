/*
 * A000142  Factorial numbers: n! = 1*2*3*4*...*n (order of symmetric group
 *          S_n, number of permutations of n letters).
 * https://oeis.org/A000142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, easy, nonn, nice
 * Formula:   a(n) = n*a(n-1), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    for (i = 2; i <= n; i++)
        r *= (seq_t)i;
    return r;
}
