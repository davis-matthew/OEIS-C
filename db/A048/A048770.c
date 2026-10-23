/*
 * A048770  Partial sums of A048694.
 * https://oeis.org/A048770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+6, with a(0)=1, a(1)=8
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A048770 (partial sums of A048694). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)8;   /* a(1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)2 * a1 + a0 + (seq_t)6; /* recurrence */
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
