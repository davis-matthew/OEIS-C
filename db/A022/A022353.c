/*
 * A022353  Fibonacci sequence beginning 0, 19.
 * https://oeis.org/A022353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19 * F(n), where F(0)=0, F(1)=1 (the standard Fibonacci
 *            sequence)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    return f1 * (seq_t)19;
}
