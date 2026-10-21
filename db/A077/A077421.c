/*
 * A077421  Chebyshev sequence U(n,11)=S(n,22) with Diophantine property.
 * https://oeis.org/A077421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - a(n-2), with a(0)=1, a(1)=22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)22;

    seq_t prevprev = (seq_t)1;   /* a(0)   */
    seq_t prev    = (seq_t)22;  /* a(1)   */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t curr = ((seq_t)22) * prev - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;   /* this is a(n) */
}
