/*
 * A221569  Number of 0..4 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) -3*a(n-2) + a(n-3) + 15*a(n-4) +3*a(n-5) for n>6
   Initial values: a(1)=0, a(2)=17, a(3)=59, a(4)=289, a(5)=1293, a(6)=5913 */

seq_t a(idx_t n)
{
    /* return quick for the first six terms */
    if (n <= 6)
    {
        static const seq_t init[6] = {
            (seq_t)0,
            (seq_t)17,
            (seq_t)59,
            (seq_t)289,
            (seq_t)1293,
            (seq_t)5913
        };
        return init[n - 1];
    }

    /* window holds the five previous terms:
       w[0] = a(i-1), w[1] = a(i-2), w[2] = a(i-3), w[3] = a(i-4), w[4] = a(i-5)   */
    seq_t w[5] = {
        (seq_t)5913,   /* a(6) */
        (seq_t)1293,   /* a(5) */
        (seq_t)289,    /* a(4) */
        (seq_t)59,     /* a(3) */
        (seq_t)17      /* a(2) */
    };

    for (idx_t i = 7; i <= n; ++i)
    {
        /* compute a(i) from the recurrence */
        seq_t next = w[0] * (seq_t)5
                    + w[2]
                    + w[3] * (seq_t)15
                    + w[4] * (seq_t)3;
        next = next - w[1] * (seq_t)3;

        /* shift the window: remove oldest, add newest */
        w[4] = w[3];
        w[3] = w[2];
        w[2] = w[1];
        w[1] = w[0];
        w[0] = next;
    }

    /* after the loop w[0] holds a(n) */
    return w[0];
}
