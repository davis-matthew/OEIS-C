/*
 * A208347  Number of nX2 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to any knight-move neighbor
 *          (colorings ignoring permutations of colors).
 * https://oeis.org/A208347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 9*a(n-2)  for n > 4;
   initial values: a(1)=2, a(2)=15, a(3)=100, a(4)=868 */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)100;
    if (n == 4) return (seq_t)868;

    seq_t prev2 = (seq_t)100;   /* a(3) */
    seq_t prev1 = (seq_t)868;   /* a(4) */
    seq_t val = 0;

    for (idx_t i = 5; i <= n; ++i) {
        val = prev1 * (seq_t)10 - prev2 * (seq_t)9;
        prev2 = prev1;
        prev1 = val;
    }
    return val;
}
