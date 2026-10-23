/*
 * A196859  Number of n X 4 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,4,2,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + a(n-5)
   - 3*a(n-6) - 6*a(n-7) - 8*a(n-8) - 8*a(n-9) - 3*a(n-10)
   with a(1)=1, a(2)=4, a(3)=12, a(4)=35, a(5)=102, a(6)=281,
   a(7)=787, a(8)=2237, a(9)=6346, a(10)=17976 */
seq_t a(idx_t n)
{
    /* Handle offset and invalid indices */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Base values for n = 1..10 */
    static const seq_t base[10] = {
        (seq_t)1, (seq_t)4, (seq_t)12, (seq_t)35,
        (seq_t)102, (seq_t)281, (seq_t)787, (seq_t)2237,
        (seq_t)6346, (seq_t)17976
    };

    if (n <= 10) {
        return base[n - 1];
    }

    /* Use a ring buffer to keep the last 10 terms */
    seq_t prev[10];
    unsigned i;
    for (i = 0; i < 10; ++i) {
        prev[i] = base[i];
    }

    /* Compute terms from 11 up to n */
    for (idx_t k = 11; k <= n; ++k) {
        /* prev[0] = a(k-10), ..., prev[9] = a(k-1) */
        seq_t newterm = ( (seq_t)3 * prev[9]
                         - prev[8]
                         + (seq_t)2 * prev[7]
                         - prev[6]
                         + prev[5]
                         - (seq_t)3 * prev[4]
                         - (seq_t)6 * prev[3]
                         - (seq_t)8 * prev[2]
                         - (seq_t)8 * prev[1]
                         - (seq_t)3 * prev[0] );

        /* Shift the buffer: drop a(k-10), slide left */
        for (i = 0; i < 9; ++i) {
            prev[i] = prev[i + 1];
        }
        prev[9] = newterm;
    }

    return prev[9];
}
