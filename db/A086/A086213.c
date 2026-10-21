/*
 * A086213  Tribonacci numbers that start with first three cubes.
 * https://oeis.org/A086213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0)=1, a(1)=8, a(2)=27
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)27;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)27;
    seq_t a3 = a0 + a1 + a2;

    for (idx_t i = 3; i <= n; ++i)
    {
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a0 + a1 + a2;
    }
    return a2;
}
