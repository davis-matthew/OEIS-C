/*
 * A097838  First differences of Chebyshev polynomials S(n,51) = A097836(n)
 *          with Diophantine property.
 * https://oeis.org/A097838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 50, * a(n) = 51*a(n-1) - a(n-2) for n >= 2 *
 *            (this follows directly from the OEIS definition * a(n) = S(n,51)
 *            - S(n-1,51) and the recurrence * a(n) = 51*a(n-1) - a(n-2) with
 *            a(0)=1, a(1)=50)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)50;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)51 * prev1;  /* 51 * a(n-1) */
        cur -= prev2;             /* minus a(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
