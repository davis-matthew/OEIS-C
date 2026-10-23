/*
 * A021844  Expansion of 1/((1-x)*(1-4*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A021844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 166*a(n-2) + 432*a(n-3) - 288*a(n-4) with
 *            initial values a(0)=1, a(1)=23, a(2)=363, a(3)=4963
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)363;
    if (n == 3) return (seq_t)4963;

    /* A small ring buffer to keep the four most recent values. */
    seq_t buf[4] = { (seq_t)1,  (seq_t)23,  (seq_t)363,  (seq_t)4963 };
    idx_t i;

    for (i = 4; i <= n; ++i)
    {
        /* Compute the next term using the recurrence. */
        seq_t next = (seq_t)23 * buf[3]
                   - (seq_t)166 * buf[2]
                   + (seq_t)432 * buf[1]
                   - (seq_t)288 * buf[0];

        /* Shift the buffer: discard oldest and append new value. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }

    return buf[3];
}
