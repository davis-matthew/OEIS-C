/*
 * A095265  A sequence generated from a 4th degree Pascal's Triangle
 *          polynomial.
 * https://oeis.org/A095265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=22, a(3)=103, a(4)=284,
 *            a(n+4)=4*a(n+3)-6*a(n+2)+4*a(n+1)-a(n) for n≥1
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A095265 */
seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)103;
    if (n == 4) return (seq_t)284;

    /* a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = (seq_t)1;      /* a(1) */
    seq_t a2 = (seq_t)22;     /* a(2) */
    seq_t a3 = (seq_t)103;    /* a(3) */
    seq_t a4 = (seq_t)284;    /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 4 * a4 - 6 * a3 + 4 * a2 - a1;
        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;  /* a4 now holds a(n) */
}
