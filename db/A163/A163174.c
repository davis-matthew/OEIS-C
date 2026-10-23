/*
 * A163174  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 24
   a(2) = 552
   a(3) = 12696
   a(4) = 291732
   a(n) = 22*a(n-1) + 22*a(n-2) + 22*a(n-3) - 253*a(n-4)   for n >= 5  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)552;
    if (n == 3) return (seq_t)12696;
    if (n == 4) return (seq_t)291732;

    /* sliding window: a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)24;          /* a(n-4) */
    seq_t a1 = (seq_t)552;         /* a(n-3) */
    seq_t a2 = (seq_t)12696;       /* a(n-2) */
    seq_t a3 = (seq_t)291732;      /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a4 = (seq_t)22 * a3
                + (seq_t)22 * a2
                + (seq_t)22 * a1
                - (seq_t)253 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
