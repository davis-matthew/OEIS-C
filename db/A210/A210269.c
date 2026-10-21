/*
 * A210269  Half the number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock
 *          having two or three distinct clockwise edge differences.
 * https://oeis.org/A210269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 12*a(n-3) Base cases: a(1)=35, a(2)=273,
 *            a(3)=2130 The recurrence requires only the three preceding
 *            values, so the sequence can be generated in O(n) time without
 *            dynamic memory allocation
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset safely */
    if (n < 1) return 0;      /* undefined for n < 1 */

    /* Base cases */
    if (n == 1) return (seq_t)35;
    if (n == 2) return (seq_t)273;
    if (n == 3) return (seq_t)2130;

    /* Sliding window of the last three values:
       prev1 = a(i-1), prev2 = a(i-2), prev3 = a(i-3)    */
    seq_t prev1 = (seq_t)2130;  /* a(3) */
    seq_t prev2 = (seq_t)273;   /* a(2) */
    seq_t prev3 = (seq_t)35;    /* a(1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = (seq_t)8 * prev1 - (seq_t)12 * prev3;
        /* shift the window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* a(n) */
}
