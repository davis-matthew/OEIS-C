/*
 * A168103  a(n) = sum of natural numbers m such that n - 4 <= m <= n + 4.
 * https://oeis.org/A168103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=n-4}^{n+4} m For n < 4: a(n) = (n+4)(n+5)/2 For n
 *            >= 4: a(n) = 9*n (Because the window then has length 9 and
 *            consists of 9 consecutive integers.)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
    {
        /* n is 0,1,2,3: compute (n+4)*(n+5)/2 exactly */
        seq_t a = (seq_t)(n + 4);
        seq_t b = (seq_t)(n + 5);
        return a * b / 2;
    }
    else
    {
        /* n >= 4: all terms in the sum are non‑negative and the window
           has exactly 9 integers from n-4 to n+4. The average is n, hence
           the sum is 9*n. */
        return (seq_t)n * 9;
    }
}
