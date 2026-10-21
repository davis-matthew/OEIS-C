/*
 * A094633  A Lucas Jacobsthal product.
 * https://oeis.org/A094633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 7*a(n-2) + 2*a(n-3) - 4*a(n-4) with a(0)=2,
 *            a(1)=1, a(2)=9, a(3)=20
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Lucas Jacobsthal product a(n) = L(n)*J(n+1) */
seq_t a(idx_t n)
{
    /* return the initial values directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)20;

    /* rolling variables for a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t a0 = (seq_t)2;   /* a(i-4) */
    seq_t a1 = (seq_t)1;   /* a(i-3) */
    seq_t a2 = (seq_t)9;   /* a(i-2) */
    seq_t a3 = (seq_t)20;  /* a(i-1) */
    seq_t next;

    /* compute up to the requested index */
    for (idx_t i = 4; i <= n; ++i) {
        next = a3 + (seq_t)7 * a2 + (seq_t)2 * a1 - (seq_t)4 * a0;
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
