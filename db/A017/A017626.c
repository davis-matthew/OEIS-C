/*
 * A017626  a(n) = (12*n + 8)^10.
 * https://oeis.org/A017626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 8)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 12*n + 8 in the target integer type */
    seq_t base = (seq_t)12 * n + (seq_t)8;
    seq_t res  = (seq_t)1;

    /* Compute base^10 iteratively; intermediate values never overflow
       if the final result fits in seq_t, because base^k <= base^10. */
    for (int i = 0; i < 10; ++i)
        res *= base;

    return res;
}
