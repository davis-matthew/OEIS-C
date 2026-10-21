/*
 * A077870  Expansion of (1-x)^(-1)/(1-x+2*x^3).
 * https://oeis.org/A077870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - 2*a(n-3) + 2*a(n-4) for n ≥ 4 Initial
 *            values: a(0)=1, a(1)=2, a(2)=3, a(3)=2. This recurrence follows
 *            from the generating function (1‑x)⁻¹/(1‑x+2x³)
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 2;

    /* Start with a(0), a(1), a(2), a(3). */
    seq_t a0 = 1;           /* a(n-4) for the first iteration (n=4) */
    seq_t a1 = 2;           /* a(n-3)   */
    seq_t a2 = 3;           /* a(n-2)   */
    seq_t a3 = 2;           /* a(n-1)   */

    /* Iterate from 4 up to n, updating the four sliding values. */
    /* idx_t is unsigned; the loop condition is safe for large n. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) - a(i-2) - 2*a(i-3) + 2*a(i-4) */
        seq_t next = 2 * a3 - a2 - 2 * a1 + 2 * a0;

        /* Shift the window: a0<-a1, a1<-a2, a2<-a3, a3<-next */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    /* After the loop, a3 holds a(n). */
    return a3;
}
