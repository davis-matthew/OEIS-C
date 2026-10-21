/*
 * A082651  Positive integer values of n such that 5n^2+11 is a square.
 * https://oeis.org/A082651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-2) - a(n-4); a(1)=1, a(2)=7, a(3)=25, a(4)=127
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)127;

    /* last[0..3] holds a(i-4), a(i-3), a(i-2), a(i-1) respectively */
    seq_t last[4];
    last[0] = (seq_t)1;   /* a(1) */
    last[1] = (seq_t)7;   /* a(2) */
    last[2] = (seq_t)25;  /* a(3) */
    last[3] = (seq_t)127; /* a(4) */

    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 18*a(i-2) - a(i-4) */
        next = last[2] * (seq_t)18;
        next -= last[0];

        /* shift window for the next iteration */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = next;
    }

    /* last[3] now holds a(n) */
    return last[3];
}
