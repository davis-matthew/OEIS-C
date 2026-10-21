/*
 * A052608  E.g.f. (1-x)/(1-2x-x^2).
 * https://oeis.org/A052608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   (-2-n^2-3*n)*a(n) +(-4-2*n)*a(n+1) +a(n+2)=0 a(0)=1, a(1)=1
 *            Recurrence derived for a(n) = (2 + n^2 + 3n)*a(n) + (4 +
 *            2n)*a(n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* iteratively compute using the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n)   */
    seq_t a1 = (seq_t)1;   /* a(n+1) */

    for (idx_t i = 0; i < n - 1; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t coeff0 = (seq_t)2 + i_seq * i_seq + (seq_t)3 * i_seq;  /* 2 + i^2 + 3i */
        seq_t coeff1 = (seq_t)4 + (seq_t)2 * i_seq;                   /* 4 + 2i   */
        seq_t a_next = coeff0 * a0 + coeff1 * a1;                     /* a(n+2)   */

        /* slide window: a(n) <- a(n+1), a(n+1) <- a(n+2) */
        a0 = a1;
        a1 = a_next;
    }

    /* After loop, a1 holds a(n) */
    return a1;
}
