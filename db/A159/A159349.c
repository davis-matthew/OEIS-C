/*
 * A159349  Transform of the finite sequence (1, 0, -1, 0, 1, 0, -1) by the
 *          T_{0,0} transformation (see link).
 * https://oeis.org/A159349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159349:
   a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 4, a(4) = 11, a(5) = 24, a(6) = 56,
   a(7) = 129, a(8) = 300.
   For n >= 9: a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3). */

seq_t a(idx_t n) {
    /* Initial terms for n = 0..8 */
    static const seq_t init[9] = { 1, 1, 1, 4, 11, 24, 56, 129, 300 };

    if (n <= 8) {
        return init[n];
    }

    /* Use only three previous values: a(i-3), a(i-2), a(i-1) */
    seq_t prev3 = init[6];   /* a(i-3) when i==9 -> a(6) */
    seq_t prev2 = init[7];   /* a(i-2) when i==9 -> a(7) */
    seq_t prev1 = init[8];   /* a(i-1) when i==9 -> a(8) */
    seq_t cur;

    for (idx_t i = 9; i <= n; ++i) {
        cur = (seq_t)3 * prev1 - (seq_t)2 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
