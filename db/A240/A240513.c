/*
 * A240513  Number of n X 2 0..1 arrays with no element equal to exactly two
 *          horizontal and vertical neighbors, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): for n ≥ 6
 *   a(n) = 2*a(n-1) + a(n-2) – a(n-3) – 2*a(n-4) + a(n-5)
 * with initial values
 *   a(1)=2, a(2)=3, a(3)=6, a(4)=10, a(5)=21
 *
 * This routine computes the term a(n) by a straightforward
 * recurrence that only keeps the last five values in memory.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* below the offset */
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)6;
    if (n == 4)
        return (seq_t)10;
    if (n == 5)
        return (seq_t)21;

    /* last five terms: a(n‑5), a(n‑4), a(n‑3), a(n‑2), a(n‑1) */
    seq_t a1 = 2;   /* a(n‑5) */
    seq_t a2 = 3;   /* a(n‑4) */
    seq_t a3 = 6;   /* a(n‑3) */
    seq_t a4 = 10;  /* a(n‑2) */
    seq_t a5 = 21;  /* a(n‑1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2 * a5 + a4 - a3 - 2 * a2 + a1;

        /* shift values for next step */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5; /* now a5 holds a(n) */
}
