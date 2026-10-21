/*
 * A063522  a(n) = n*(5*n^2 - 3)/2.
 * https://oeis.org/A063522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n^2 - 3)/2
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Convert the index to the sequence type once. */
    seq_t sn = (seq_t)n;

    /* Compute 5*n^2 - 3 */
    seq_t sn_sq = sn * sn;              /* n^2 */
    seq_t five_n_sq = sn_sq * ((seq_t)5); /* 5*n^2 */
    seq_t expr = five_n_sq - ((seq_t)3);  /* 5*n^2 - 3 */

    /* Multiply by n and divide by 2 (exact division). */
    seq_t prod = sn * expr;             /* n*(5*n^2 - 3) */
    return prod / ((seq_t)2);
}
