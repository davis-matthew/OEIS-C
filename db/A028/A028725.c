/*
 * A028725  a(n) = floor(n/2) * floor((n-1)/2) * floor((n-2)/2) *
 *          floor((n-3)/2) * floor((n-4)/2) / 12.
 * https://oeis.org/A028725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) * floor((n-1)/2) * floor((n-2)/2) * *
 *            floor((n-3)/2) * floor((n-4)/2) / 12
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 278, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n < 5 the product contains a zero factor */
    if (n < 5)
        return 0;

    seq_t f0 = (seq_t)n >> 1;
    seq_t f1 = ((seq_t)(n - 1)) >> 1;
    seq_t f2 = ((seq_t)(n - 2)) >> 1;
    seq_t f3 = ((seq_t)(n - 3)) >> 1;
    seq_t f4 = ((seq_t)(n - 4)) >> 1;

    seq_t prod = f0 * f1 * f2 * f3 * f4;
    return prod / 12;
}
