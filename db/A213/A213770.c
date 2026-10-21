/*
 * A213770  Antidiagonal sums of the convolution array A213768.
 * https://oeis.org/A213770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5), with
 *            initial values a(1)=1, a(2)=7, a(3)=23, a(4)=58, a(5)=126
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the fixed initial values directly. */
    if (n == 1) return 1;
    if (n == 2) return 7;
    if (n == 3) return 23;
    if (n == 4) return 58;
    if (n == 5) return 126;

    /* Keep the last five terms of the sequence. */
    seq_t a0 = 1;   /* a(n-5)  */
    seq_t a1 = 7;   /* a(n-4)  */
    seq_t a2 = 23;  /* a(n-3)  */
    seq_t a3 = 58;  /* a(n-2)  */
    seq_t a4 = 126; /* a(n-1)  */

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute the next term without any intermediate underflow
           (the expression is guaranteed non‑negative). */
        seq_t next = 4 * a4 + a2 + 2 * a1;
        next -= 5 * a3;
        next -= a0;

        /* Shift the window forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
