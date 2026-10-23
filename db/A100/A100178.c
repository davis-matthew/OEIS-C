/*
 * A100178  Structured hexagonal diamond numbers (vertex structure 5).
 * https://oeis.org/A100178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(8*n^3 - 6*n^2 + 4*n) Recurrence: a(n) = 4*a(n-1) -
 *            6*a(n-2) + 4*a(n-3) - a(n-4) for n > 4 Initial values: a(1)=1,
 *            a(2)=8, a(3)=29, a(4)=72
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1477, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)29;
    if (n == 4) return (seq_t)72;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)8;   /* a(2) */
    seq_t a3 = (seq_t)29;  /* a(3) */
    seq_t a4 = (seq_t)72;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 4 * a4 - 6 * a3 + 4 * a2 - a1; /* recurrence */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
