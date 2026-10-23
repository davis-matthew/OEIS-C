/*
 * A123304  Number of edge covers for the circular ladder (n-prism graph) C_n
 *          X K_2.
 * https://oeis.org/A123304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 9*a(n-2) + a(n-3) - 2*a(n-4) initial values:
 *            a(0)=4, a(1)=5, a(2)=43, a(3)=263
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    seq_t a0 = (seq_t)4;   /* a(0) */
    seq_t a1 = (seq_t)5;   /* a(1) */
    seq_t a2 = (seq_t)43;  /* a(2) */
    seq_t a3 = (seq_t)263; /* a(3) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)5 * a3 + (seq_t)9 * a2 + a1 - (seq_t)2 * a0;
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
