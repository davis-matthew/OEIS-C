/*
 * A135575  a(n) = A135574(n+1) - 2*A135574(n).
 * https://oeis.org/A135575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) + 2*a(n-1) + a(n-2) + 2*a(n-3) + a(n-4) + 2*a(n-5) = 0
 *            a(0)=0, a(1)=3, a(2)=-5, a(3)=9, a(4)=-16
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)-5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)-16;

    /* Sliding window of the last five values.
       buf[0] = a(k-5), buf[1] = a(k-4), buf[2] = a(k-3),
       buf[3] = a(k-2), buf[4] = a(k-1) for the current k. */
    seq_t buf[5] = {(seq_t)0, (seq_t)3, (seq_t)-5, (seq_t)9, (seq_t)-16};

    idx_t i;
    for (i = 5; i <= n; ++i) {
        /* Compute a(i) using the recurrence.
           Group the terms to avoid overflow on 64‑bit builds. */
        seq_t sum_two = buf[4] + buf[2] + buf[0];         /* terms with coefficient 2 */
        seq_t sum_one  = buf[3] + buf[1];                 /* terms with coefficient 1 */
        seq_t next = -((seq_t)2 * sum_two + sum_one);

        /* Shift the window for the next iterate. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
