/*
 * A177277  Partial sums of round(n^2/28).
 * https://oeis.org/A177277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*(n+2)*(2*n-1)/168)
 * Range:     u8 n <= 27, u16 n <= 176, u32 n <= 905, u64 n <= 905,
 *            u128 n <= 905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use a 128-bit intermediate to avoid overflow for any reasonable n. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 num = nn * (nn + 2) * (2 * nn - 1) + 84;  /* +84 = 168/2 for rounding */
    seq_t res = (seq_t)(num / 168);
    return res;
}
