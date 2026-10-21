/*
 * A097835  First differences of Chebyshev polynomials S(n,27) = A097781(n)
 *          with Diophantine property.
 * https://oeis.org/A097835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - a(n-2), a(0) = 1, a(1) = 26
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;

    /* a_prev   = a(n-1)  (initially a(1))
       a_prev2  = a(n-2)  (initially a(0)) */
    seq_t a_prev  = (seq_t)26;
    seq_t a_prev2 = (seq_t)1;
    seq_t cur;

    /* Iterate up to the requested n. */
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)27 * a_prev - a_prev2;
        a_prev2 = a_prev;
        a_prev  = cur;
    }

    return a_prev;
}
