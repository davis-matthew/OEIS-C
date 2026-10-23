/*
 * A041635  Denominators of continued fraction convergents to sqrt(336).
 * https://oeis.org/A041635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 110*a(n-2) - a(n-4) for n>3, with a(0)=1, a(1)=3,
 *            a(2)=109, a(3)=330
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)109;
    if (n == 3) return (seq_t)330;

    /* Use four registers to iterate the recurrence */
    seq_t prev4 = (seq_t)1;   /* a(n-4) */
    seq_t prev3 = (seq_t)3;   /* a(n-3) */
    seq_t prev2 = (seq_t)109; /* a(n-2) */
    seq_t prev1 = (seq_t)330; /* a(n-1) */
    seq_t current = 0;

    for (idx_t i = 4; i <= n; ++i) {
        current = (seq_t)110 * prev2 - prev4;  /* a(i) = 110*a(i-2) - a(i-4) */
        /* Shift the registers for the next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
