/*
 * A095661  Fifth column (m=4) of (1,3)-Pascal triangle A095660.
 * https://oeis.org/A095661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+12)*(n+3)*(n+2)*(n+1) / 24 (OEIS A095661)
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the generic type first */
    seq_t t = (seq_t)n;

    /* Compute the numerator: (n+12)(n+3)(n+2)(n+1) */
    seq_t prod = t + 12;
    prod *= t + 3;
    prod *= t + 2;
    prod *= t + 1;

    /* Divide by the constant factor 24 */
    return prod / 24;
}
