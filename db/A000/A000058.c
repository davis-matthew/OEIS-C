/*
 * A000058  Sylvester's sequence: a(n+1) = a(n)^2 - a(n) + 1, with a(0) = 2.
 * https://oeis.org/A000058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, core, changed
 * Formula:   a(n) = a(n-1)^2 - a(n-1) + 1, a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 2;
    idx_t i;
    for (i = 0; i < n; i++)
        x = x * (x - 1) + 1;
    return x;
}
