/*
 * A097731  Chebyshev U(n,x) polynomial evaluated at x=99 gives 2*7^2+1.
 * https://oeis.org/A097731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*99*a(n-1) - a(n-2), a(0) = 1, a(1) = 198
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)198;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)198; /* a(1) */
    seq_t next;
    for (idx_t i = 2; i <= n; ++i) {
        /* next = 198 * a1 - a0 */
        next = (seq_t)198 * a1 - a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
