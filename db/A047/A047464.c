/*
 * A047464  Numbers that are congruent to {0, 2, 4} mod 8.
 * https://oeis.org/A047464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor((n-1)/3)+2*n-2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the unused 0-th index gracefully */
    if (n == 0)
        return 0;

    /* Compute floor((n-1)/3) using integer division */
    idx_t k = (n - 1) / 3;

    /* Cast to seq_t before arithmetic to preserve the correct width */
    seq_t kk = (seq_t)k;
    seq_t nn = (seq_t)n;

    /* a(n) = 2*k + 2*n - 2 */
    seq_t result = 2 * kk + 2 * nn - 2;
    return result;
}
