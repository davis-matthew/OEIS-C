/*
 * A097832  Partial sums of Chebyshev sequence S(n,19) = U(n,19/2) =
 *          A078368(n).
 * https://oeis.org/A097832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - a(n-2) + 1 with a(0)=1, a(1)=20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 20;

    seq_t a_nm2 = 1;   /* a(n-2) */
    seq_t a_nm1 = 20;  /* a(n-1) */
    seq_t a_n   = 0;   /* a(n)   */

    for (idx_t i = 2; i <= n; ++i) {
        a_n = 19 * a_nm1 - a_nm2 + 1;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
