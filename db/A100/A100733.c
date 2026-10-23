/*
 * A100733  a(n) = (4*n)!.
 * https://oeis.org/A100733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (4*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t total = n * (idx_t)4;
    seq_t result = 1;
    for (idx_t i = 2; i <= total; ++i)
    {
        result *= (seq_t)i;
    }
    return result;
}
