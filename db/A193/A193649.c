/*
 * A193649  Q-residue of the (n+1)st Fibonacci polynomial, where Q is the
 *          triangular array (t(i,j)) given by t(i,j)=1. (See Comments.)
 * https://oeis.org/A193649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) (for n ≥ 3), with a(0) =
 *            1, a(1) = 1, a(2) = 3. The sequence is defined by the Q‑residue
 *            of the (n+1)st Fibonacci polynomial with Q(i,j) = 1 for all i,j.
 *            The linear recurrence follows from the generating function
 *            -(1+x)*(2*x-1)/((x-1)*(4*x^2+x-1))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)3; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute a(i) = 2*a2 + 3*a1 - 4*a0 */
        seq_t cur = (seq_t)2 * a2 + (seq_t)3 * a1;
        cur -= (seq_t)4 * a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2; /* a(n) */
}
