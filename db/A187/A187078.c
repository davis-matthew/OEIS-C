/*
 * A187078  38 followed by n ones.
 * https://oeis.org/A187078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A187078: 38 followed by n ones.
 *  Recurrence:  a(0) = 38,
 *                a(n) = 10 * a(n-1) + 1   (n > 0)
 */
seq_t a(idx_t n) {
    /* Start value a(0) */
    seq_t val = 38;
    /* Iterate the recurrence up to n */
    idx_t i;
    for (i = 1; i <= n; ++i) {
        val = val * 10 + 1;
    }
    return val;
}
