/*
 * A047990  a(n+1) = a(n) + (n^2 + 1)*a(n-1).
 * https://oeis.org/A047990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + ((n-1)^2 + 1) * a(n-2); a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t factor = ((seq_t)(i - 1)) * ((seq_t)(i - 1)) + (seq_t)1;
        cur = prev + factor * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
