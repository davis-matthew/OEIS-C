/*
 * A121555  Number of 1-cell columns in all deco polyominoes of height n.
 * https://oeis.org/A121555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = n*a(n-1) + (n-2)!*(n-2) for n >= 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t a_prev = (seq_t)1; /* a(1) */
    seq_t fact   = (seq_t)1; /* (k-2)! for k = 2 => 0! */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t term = a_prev * (seq_t)k;            /* n*a(n-1)   */
        seq_t add  = fact * (seq_t)(k - 2);       /* (k-2)!*(k-2) */
        seq_t a_k  = term + add;

        if (k == n)
            return a_k;

        a_prev = a_k;
        fact   = fact * (seq_t)(k - 1);           /* update to (k-1)! */
    }

    /* unreachable, but keep compiler happy */
    return (seq_t)1;
}
