/*
 * A186875  Number of (n+2) X 5 0..3 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 30, u64 n <= 73, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186875
   Recurrence: a(n)=2*a(n-1)-a(n-2)+3*a(n-3)-6*a(n-4)+3*a(n-5)+3*a(n-6)-3*a(n-7)
   for n>11, with initial values:
   a(1)=19236, a(2)=1201, a(3)=6712, a(4)=20152, a(5)=28984,
   a(6)=32992, a(7)=37576, a(8)=46624, a(9)=73744, a(10)=138808, a(11)=251563.
*/
seq_t a(idx_t n)
{
    /* initial 11 terms */
    static const seq_t init[11] = {
        19236, 1201, 6712, 20152, 28984,
        32992, 37576, 46624, 73744, 138808, 251563
    };

    if (n == 0) return 0;          /* undefined for n<1, but harmless */
    if (n <= 11) return init[n - 1];

    /* keep the last 7 values: a(i-7) ... a(i-1) */
    seq_t vals[7];
    for (idx_t i = 0; i < 7; ++i)
        vals[i] = init[4 + i];  /* init[4] = a(5), ..., init[10] = a(11) */

    seq_t result = 0;
    for (idx_t i = 12; i <= n; ++i) {
        /* compute a(i) using the recurrence, avoiding unsigned underflow */
        seq_t pos = (seq_t)2 * vals[6] + (seq_t)3 * vals[4] + (seq_t)3 * vals[2] + (seq_t)3 * vals[1];
        seq_t neg = vals[5] + (seq_t)6 * vals[3] + (seq_t)3 * vals[0];
        result = pos - neg;

        /* shift left to make room for the new value */
        for (idx_t j = 0; j < 6; ++j)
            vals[j] = vals[j + 1];
        vals[6] = result;
    }

    return result;
}
