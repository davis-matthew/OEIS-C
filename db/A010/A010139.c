/*
 * A010139  Continued fraction for sqrt(53).
 * https://oeis.org/A010139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9972, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 7; for n > 0: a(n) = period[(n-1) mod 5] with period {3,1,1,3,14} */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)7;
    }
    static const seq_t period[5] = { (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)14 };
    idx_t r = (n - 1) % 5;
    return period[r];
}
