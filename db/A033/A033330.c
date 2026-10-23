/*
 * A033330  a(n) = floor(10/n).
 * https://oeis.org/A033330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = ⌊10 / n⌋ for n ≥ 1.  For n = 0 the value is undefined; it returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return (seq_t)10 / (seq_t)n;
}
