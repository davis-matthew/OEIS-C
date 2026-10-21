/*
 * A205979  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) + 35*a(n-3) + a(n-4) + 3*a(n-5) +
 *            a(n-6) + 2*a(n-7) with initial values a(1)=81, a(2)=423,
 *            a(3)=2232, a(4)=11568, a(5)=60432, a(6)=315357, a(7)=1643538
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0)
        return (seq_t)0;  /* out of range, but harmless */

    /* Initial values for a(1)..a(7) */
    static const seq_t init[7] = {
        (seq_t)81,     /* a(1) */
        (seq_t)423,    /* a(2) */
        (seq_t)2232,   /* a(3) */
        (seq_t)11568,  /* a(4) */
        (seq_t)60432,  /* a(5) */
        (seq_t)315357, /* a(6) */
        (seq_t)1643538/* a(7) */
    };

    if (n <= 7)
        return init[(idx_t)n - 1];

    /* Remainder: compute iteratively using a circular buffer of the last 7 values */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    seq_t next;
    for (idx_t i = 8; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        next = (seq_t)2 * buf[6]   /* a(i-1) */
             + (seq_t)10 * buf[5]  /* a(i-2) */
             + (seq_t)35 * buf[4]  /* a(i-3) */
             + buf[3]             /* a(i-4) */
             + (seq_t)3 * buf[2]   /* a(i-5) */
             + buf[1]             /* a(i-6) */
             + (seq_t)2 * buf[0];  /* a(i-7) */

        /* Shift buffer left by one position */
        for (idx_t j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = next;
    }

    return buf[6];  /* a(n) is the last element in the buffer */
}
