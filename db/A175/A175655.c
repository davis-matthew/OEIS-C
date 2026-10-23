/*
 * A175655  Eight bishops and one elephant on a 3 X 3 chessboard. G.f.:
 *          (1+x-5*x^2)/(1-3*x-x^2+6*x^3).
 * https://oeis.org/A175655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 6*a(n-3) Initial values: a(0)=1,
 *            a(1)=4, a(2)=8
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Keep last three terms */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t next = 0;

    /* Iterate from 3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        next = 3 * a2 + a1 - 6 * a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
