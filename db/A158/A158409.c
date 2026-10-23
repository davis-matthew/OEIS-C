/*
 * A158409  a(n) = 900*n - 1.
 * https://oeis.org/A158409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 900*n - 1
 * Range:     u8 none, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is linear; cast to seq_t to avoid overflow of idx_t */
    return (seq_t)900 * (seq_t)n - (seq_t)1;
}
