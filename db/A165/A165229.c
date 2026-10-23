/*
 * A165229  a(n) = 12*a(n-1) - 6*a(n-2), with a(0)=1, a(1)=6.
 * https://oeis.org/A165229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 6*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Iterate using the recurrence, keeping only the two most recent values. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t curr  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)12 - prev2 * (seq_t)6;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
