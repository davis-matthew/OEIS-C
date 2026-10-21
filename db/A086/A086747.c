/*
 * A086747  Baum-Sweet sequence: a(n) = 1 if binary representation of n
 *          contains no block of consecutive zeros of odd length; otherwise
 *          a(n) = 0.
 * https://oeis.org/A086747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if the binary representation of n contains no block of
 * consecutive zeros of odd length; otherwise a(n) = 0.
 * a(0) is defined as 0.
 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t m = n;
    unsigned long long zero_run = 0;
    while (m) {
        if ((m & (idx_t)1) == 0) {
            /* we are in a run of zeros */
            zero_run++;
        } else {
            /* run ends on encountering a 1 */
            if ((zero_run & 1ULL) != 0) return (seq_t)0;
            zero_run = 0;
        }
        m >>= 1;
    }
    /* trailing zeros after the most significant 1
       (empty run counts as 0) */
    if ((zero_run & 1ULL) != 0) return (seq_t)0;
    return (seq_t)1;
}
