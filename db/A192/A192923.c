/*
 * A192923  Coefficient of x in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) - a(n-4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first four terms directly */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;
    if (n == 3u) return (seq_t)4;

    /* Keep the last four values of the sequence
       a0 = a(k-4), a1 = a(k-3), a2 = a(k-2), a3 = a(k-1) */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)4;
    seq_t an;

    /* Compute a(4), a(5), …, a(n) iteratively */
    for (idx_t k = 4u; k <= n; ++k) {
        an = (seq_t)((seq_t)2 * a3 + (seq_t)2 * a2
                     - (seq_t)3 * a1 - a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
