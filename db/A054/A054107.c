/*
 * A054107  T(n,n-3), array T as in A054106.
 * https://oeis.org/A054107
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 6*n^2 + 23*n - 48) / 6 for n >= 6; a(3) = 1, a(4)
 *            = 3, a(5) = 8
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;  /* undefined for n < 3, return 0 as placeholder */

    /* small values by definition */
    if (n == 3) return 1;
    if (n == 4) return 3;
    if (n == 5) return 8;

    /* compute the cubic formula for n >= 6 */
    seq_t N      = (seq_t)n;
    seq_t n2     = N * N;       /* n^2 */
    seq_t n3     = N * n2;      /* n^3 */
    seq_t numer  = n3 - 6 * n2 + 23 * N - 48;
    return numer / 6;
}
