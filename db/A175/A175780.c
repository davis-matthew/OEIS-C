/*
 * A175780  Partial sums of floor(n^2/24).
 * https://oeis.org/A175780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+11)*(n-2)^2/144)
 * Range:     u8 n <= 17, u16 n <= 167, u32 n <= 1317, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n-2)^2 safely using unsigned arithmetic */
    idx_t diff = (n >= 2) ? (n - 2) : (2 - n);           /* non‑negative value */
    seq_t diff_sq = (seq_t)diff * (seq_t)diff;

    /* Compute 2*n + 11 */
    seq_t mul = (seq_t)n * 2 + 11;

    /* Final result: floor division by 144 */
    return (mul * diff_sq) / 144;
}
