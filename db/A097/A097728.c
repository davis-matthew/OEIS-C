/*
 * A097728  Chebyshev U(n,x) polynomial evaluated at x=73 = 2*6^2+1.
 * https://oeis.org/A097728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 146*a(n-1) - a(n-2), a(0)=1, a(1)=146 (Chebyshev U(n,73))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)146;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)146; /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = ((seq_t)146) * curr - prev;
        prev = curr;
        curr = next;
    }

    return curr;
}
