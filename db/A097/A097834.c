/*
 * A097834  Chebyshev polynomials S(n,27) + S(n-1,27) with Diophantine
 *          property.
 * https://oeis.org/A097834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 28, a(n) = 27*a(n-1) - a(n-2) (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = S(n,27)+S(n-1,27), linear recurrence from the gf (1+x)/(1-27x+x^2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;

    seq_t prev = (seq_t)1;          /* a(n-2)   */
    seq_t curr = (seq_t)28;         /* a(n-1)   */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = ((seq_t)27) * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
