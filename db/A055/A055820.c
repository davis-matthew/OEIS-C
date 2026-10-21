/*
 * A055820  a(n) = T(n,n-3), array T as in A055818.
 * https://oeis.org/A055820
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 3*n^2 – 10*n – 6)/6, a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1915, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for n < 3 are undefined; return 0 for safety. */
    if (n < 3u)   return 0;
    if (n == 3u)  return 1;

    seq_t nn = (seq_t)n;
    /* Compute (nn^3 + 3*nn^2) – (10*nn + 6) to avoid negative constants
       which would wrap in unsigned types. */
    seq_t term1 = nn * nn * nn + 3 * nn * nn;
    seq_t term2 = 10 * nn + 6;
    return (term1 - term2) / 6;
}
