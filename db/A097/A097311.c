/*
 * A097311  Chebyshev polynomials of the second kind, U(n,x), evaluated at
 *          x=14.
 * https://oeis.org/A097311
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - a(n-2), with a(-1)=0, a(0)=1, a(1)=28
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return (seq_t)0;
    if (n == (idx_t)0)  return (seq_t)1;
    if (n == (idx_t)1)  return (seq_t)28;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)28;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = ((seq_t)28) * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
