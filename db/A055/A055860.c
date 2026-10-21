/*
 * A055860  a(n) = A000169(n+1) if n > 0; a(0) = 0.
 * https://oeis.org/A055860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n for n > 0, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t base = (seq_t)(n + 1);
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= base;
    return result;
}
