/*
 * A112611  a(n) = 5*a(n-1) + 45*a(n-3) - 225*a(n-4), a(0)=0, a(1)=4, a(2)=24,
 *          a(3)=60, a(4)=480.
 * https://oeis.org/A112611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 45*a(n-3) - 225*a(n-4) a(0) = 0, a(1) = 4,
 *            a(2) = 24, a(3) = 60, a(4) = 480
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 24;
    if (n == 3) return 60;
    if (n == 4) return 480;

    /* Keep the last four terms needed for the recurrence:
       a_{i-4}, a_{i-3}, a_{i-2}, a_{i-1}. */
    seq_t a_prev4 = 4;   /* a(1) */
    seq_t a_prev3 = 24;  /* a(2) */
    seq_t a_prev2 = 60;  /* a(3) */
    seq_t a_prev1 = 480; /* a(4) */

    seq_t curr;
    for (idx_t i = 5; i <= n; ++i) {
        curr = 5 * a_prev1 + 45 * a_prev3 - 225 * a_prev4;
        /* Shift the window one step forward. */
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = curr;
    }
    return a_prev1;
}
