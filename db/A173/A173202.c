/*
 * A173202  Solutions y of the Mordell equation y^2 = x^3 - 3a^2 + 1 for a =
 *          0,1,2, ... (solutions x are given by the sequence A000466).
 * https://oeis.org/A173202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(8*(n-1)^2 - 3) (OEIS offset 1)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 813, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert index to seq_t once, avoiding signedness issues.
    seq_t k = (seq_t)n - 1;
    // Compute k * (8*k*k - 3)
    seq_t term = k * (((seq_t)8 * k * k) - (seq_t)3);
    return term;
}
