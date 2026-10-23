/*
 * A146123  Number of n X n arrays of squares of integers with every (n-3) X
 *          (n-3) subblock summing to 4 and every element equal to at least
 *          one neighbor.
 * https://oeis.org/A146123
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -10*a(n-2) +10*a(n-3) -5*a(n-4) + a(n-5) Offset:
 *            5, a(5)=1, a(6)=540, a(7)=4120, a(8)=12188, a(9)=31000,
 *            a(10)=67216, a(11)=129464, a(12)=228136. For n>12 recurrence
 *            holds
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;   /* undefined for n<5 */

    static const seq_t init[8] = {
        1,      /* a(5) */
        540,    /* a(6) */
        4120,   /* a(7) */
        12188,  /* a(8) */
        31000,  /* a(9) */
        67216,  /* a(10) */
        129464, /* a(11) */
        228136  /* a(12) */
    };

    if (n <= 12) return init[n - 5];

    /* Initialize sliding window with a(5)..a(12) */
    seq_t a5 = init[0];
    seq_t a6 = init[1];
    seq_t a7 = init[2];
    seq_t a8 = init[3];
    seq_t a9 = init[4];
    seq_t a10 = init[5];
    seq_t a11 = init[6];
    seq_t a12 = init[7];

    for (idx_t i = 13; i <= n; ++i) {
        seq_t next = 5 * a12 - 10 * a11 + 10 * a10 - 5 * a9 + a8;
        a8 = a9;
        a9 = a10;
        a10 = a11;
        a11 = a12;
        a12 = next;
    }
    return a12;
}
