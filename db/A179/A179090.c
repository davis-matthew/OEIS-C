/*
 * A179090  Variant of Sylvester's sequence: a(n + 1) = a(n)^2 - a(n) + 1,
 *          with a(1) = 14.
 * https://oeis.org/A179090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 14; a(n+1) = a(n)^2 - a(n) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t val = (seq_t)14;
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * val - val + (seq_t)1;
    }
    return val;
}
