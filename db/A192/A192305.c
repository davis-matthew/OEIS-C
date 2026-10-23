/*
 * A192305  0-sequence of reduction of (2n) by x^2 -> x+1.
 * https://oeis.org/A192305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A192305: 0‑sequence of reduction of (2*n) by x^2 -> x+1
 * Recurrence (order 5):
 *     a(n) = 3*a(n‑1) – a(n‑2) – 3*a(n‑3) + a(n‑4) + a(n‑5)
 * with initial values:
 *     a(1)=2, a(2)=2, a(3)=8, a(4)=16, a(5)=36
 * (n ≥ 1).  This gives the same sequence as the generating function
 * 2*x*(1-2*x+2*x^2)/((1-x)*(1-x-x^2)^2). */

seq_t a(idx_t n)
{
    /* Return the first five terms directly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)16;
    if (n == 5) return (seq_t)36;

    /* Sliding window of the last five terms.
     * a[0] .. a[4] correspond to a(n‑5) .. a(n‑1) during the loop. */
    seq_t a0 = (seq_t)2;  /* a(1) */
    seq_t a1 = (seq_t)2;  /* a(2) */
    seq_t a2 = (seq_t)8;  /* a(3) */
    seq_t a3 = (seq_t)16; /* a(4) */
    seq_t a4 = (seq_t)36; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 3 * a4 - a3 - 3 * a2 + a1 + a0;
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;  /* a(n) */
}
