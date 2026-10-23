/*
 * A130647  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+839)^2 = y^2.
 * https://oeis.org/A130647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 28, u64 n <= 65, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A130647: Nonnegative values x of solutions (x, y) to the
   Diophantine equation x^2 + (x+839)^2 = y^2.
   Recurrence: a(n) = 6*a(n-3) – a(n-6) + 1678  for n > 6
   Initial values: a(1)=0, a(2)=60, a(3)=2241, a(4)=2517, a(5)=2821, a(6)=15180. */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Directly return the first six values. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)60;
    if (n == 3) return (seq_t)2241;
    if (n == 4) return (seq_t)2517;
    if (n == 5) return (seq_t)2821;
    if (n == 6) return (seq_t)15180;

    /* Use a sliding window of six consecutive terms. */
    seq_t a1 = (seq_t)0;     /* a(i-6)   */
    seq_t a2 = (seq_t)60;    /* a(i-5)   */
    seq_t a3 = (seq_t)2241;  /* a(i-4)   */
    seq_t a4 = (seq_t)2517;  /* a(i-3)   */
    seq_t a5 = (seq_t)2821;  /* a(i-2)   */
    seq_t a6 = (seq_t)15180; /* a(i-1)   */

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 6*a(i-3) – a(i-6) + 1678 */
        seq_t next = 6 * a4 - a1 + (seq_t)1678;

        /* shift the window: move one step forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6; /* now a6 holds a(n) for n >= 7 */
}
