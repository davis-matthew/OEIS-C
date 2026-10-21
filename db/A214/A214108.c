/*
 * A214108  Number of 0..3 colorings on an nX5 array circular in the 5
 *          direction with new values 0..3 introduced in row major order.
 * https://oeis.org/A214108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 68*a(n-1) - 66*a(n-2) with a(1) = 10, a(2) = 670
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, guard */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)670;

    seq_t prev2 = (seq_t)10;            /* a(1) */
    seq_t prev1 = (seq_t)670;           /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)68 * prev1 - (seq_t)66 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
