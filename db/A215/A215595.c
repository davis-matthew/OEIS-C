/*
 * A215595  Number of strings of length n, formed from the 26-letter English
 *          alphabet, which contain the substring xy.
 * https://oeis.org/A215595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) + 26^(n-2) - a(n-2), with a(0)=a(1)=0. This
 *            recurrence counts strings of length n that contain the substring
 *            "xy"
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n < 2)
        return (seq_t)0;

    /* a_prev2 = a(n-2), a_prev1 = a(n-1) start with a(0)=a(1)=0. */
    seq_t a_prev2 = 0;
    seq_t a_prev1 = 0;

    /* pow26 holds 26^(i-2) during the loop, starts at 26^0 = 1. */
    seq_t pow26 = 1;

    seq_t result = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        result = a_prev1;    /* a(i-1)           */
        result *= 26;        /* 26*a(i-1)        */
        result += pow26;     /* +26^(i-2)        */
        result -= a_prev2;   /* -a(i-2)          */

        /* Shift for next iteration. */
        a_prev2 = a_prev1;
        a_prev1 = result;

        /* Update power for next step: 26^(i-1). */
        pow26 *= 26;
    }

    return result;
}
