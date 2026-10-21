/*
 * A230470  Number of 2 X n 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 12*a(n-3) + 20*a(n-4) + 24*a(n-5),
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=36, a(5)=216
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset handling */
    if (n < 1)
        return 0;

    /* base cases */
    if (n <= 5) {
        switch ((int)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)0;
            case 4: return (seq_t)36;
            case 5: return (seq_t)216;
            default: return (seq_t)0;
        }
    }

    /* rolling buffer for the five most recent terms */
    seq_t buf[5];
    buf[0] = (seq_t)0;   /* a(1) */
    buf[1] = (seq_t)0;   /* a(2) */
    buf[2] = (seq_t)0;   /* a(3) */
    buf[3] = (seq_t)36;  /* a(4) */
    buf[4] = (seq_t)216; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* compute next term using the recurrence
           in a way that avoids underflow for unsigned types */
        seq_t next = (seq_t)6 * buf[4]    /* 6*a(n-1) */
                   + (seq_t)20 * buf[1]   /* +20*a(n-4) */
                   + (seq_t)24 * buf[0]   /* +24*a(n-5) */
                   - buf[3]              /* - a(n-2) */
                   - (seq_t)12 * buf[2]; /* -12*a(n-3) */

        /* shift buffer: remove oldest term, append new term */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }
    return buf[4];   /* a(n) */
}
