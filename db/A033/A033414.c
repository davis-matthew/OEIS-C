/*
 * A033414  a(n) = floor(94/n).
 * https://oeis.org/A033414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) = floor(94/n) */
seq_t a(idx_t n)
{
    /* Return 0 for n==0 (usage below offset is undefined) */
    if (n == 0)
        return 0;
    /* Compute floor(94 / n) using idx_t to avoid overflow and cast to seq_t. */
    idx_t v = 94 / n;
    return (seq_t)v;
}
