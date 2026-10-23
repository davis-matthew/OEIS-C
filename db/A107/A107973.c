/*
 * A107973  Numbers of the form a^2 + b for a= 21 to 40 and b= 20 to 1 step
 *          -1.
 * https://oeis.org/A107973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = (20 + n)^2 + (21 - n) for 1 ≤ n ≤ 20 (otherwise 0)
 * Range:     u8 none, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Ensure index is within the finite defined range */
    if (n < 1 || n > 20) {
        return (seq_t)0;
    }

    /* Compute a = 20 + n and b = 21 - n as seq_t */
    seq_t a_val = (seq_t)(20 + n);
    seq_t b_val = (seq_t)(21 - n);

    /* Return a^2 + b */
    return a_val * a_val + b_val;
}
