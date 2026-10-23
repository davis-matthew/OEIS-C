/*
 * A041647  Denominators of continued fraction convergents to sqrt(342).
 * https://oeis.org/A041647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=73, a(3)=148, a(n)=74*a(n-2)-a(n-4) for n>3
 *            (Denominators of continued fraction convergents to sqrt(342).)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 73;
    if (n == 3) return 148;

    /* Variables to keep the four most recent terms:
       prev4 = a(i-4), prev3 = a(i-3), prev2 = a(i-2), prev1 = a(i-1)   */
    seq_t prev4 = 1;   /* a(0) */
    seq_t prev3 = 2;   /* a(1) */
    seq_t prev2 = 73;  /* a(2) */
    seq_t prev1 = 148; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = 74 * prev2 - prev4;  /* recurrence */
        /* Shift the window of four values */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = val;
    }
    return prev1;
}
