/*
 * A028087  Expansion of 1/((1-3x)(1-6x)(1-10x)(1-12x)).
 * https://oeis.org/A028087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 336*a(n-2) + 1476*a(n-3) - 2160*a(n-4) with
 *            initial values a(0)=1, a(1)=31, a(2)=625, a(3)=10435
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)625;
    if (n == 3) return (seq_t)10435;

    /* Constants for the recurrence.  Use seq_t so the arithmetic is
       performed in the correct type for each build. */
    const seq_t C1 = (seq_t)31;
    const seq_t C2 = (seq_t)336;
    const seq_t C3 = (seq_t)1476;
    const seq_t C4 = (seq_t)2160;

    /* Keep the last four coefficients. */
    seq_t a0 = (seq_t)1;     /* a(i-4)  */
    seq_t a1 = (seq_t)31;    /* a(i-3)  */
    seq_t a2 = (seq_t)625;   /* a(i-2)  */
    seq_t a3 = (seq_t)10435; /* a(i-1)  */
    seq_t an;                /* a(i)    */

    /* Iterate from i = 4 up to n, using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        an = C1 * a3 - C2 * a2 + C3 * a1 - C4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
