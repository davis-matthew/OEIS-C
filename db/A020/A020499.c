/*
 * A020499  Expansion of 1/((1-5x)(1-9x)(1-11x)).
 * https://oeis.org/A020499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 199*a(n-2) + 495*a(n-3) a(0) = 1, a(1) = 25,
 *            a(2) = 426
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)25;
    if (n == 2)
        return (seq_t)426;

    /* Keep the last three terms of the recurrence. */
    seq_t t0 = (seq_t)1;          /* a(n-3) */
    seq_t t1 = (seq_t)25;         /* a(n-2) */
    seq_t t2 = (seq_t)426;        /* a(n-1) */
    seq_t tmp;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute a(i) = 25*t2 + 495*t0 - 199*t1
           Adding the positive terms first avoids unsigned underflow. */
        tmp = (seq_t)25 * t2;
        tmp += (seq_t)495 * t0;
        tmp -= (seq_t)199 * t1;

        /* Shift the window forward. */
        t0 = t1;
        t1 = t2;
        t2 = tmp;
    }

    return t2;
}
