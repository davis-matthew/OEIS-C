/*
 * A033378  a(n) = floor(58/n).
 * https://oeis.org/A033378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(58 / n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For safety, handle n == 0. */
    if (n == 0) {
        return 0;
    }
    /* Integer division in C truncates toward zero; for positive values this is floor. */
    return ((seq_t)58) / ((seq_t)n);
}
