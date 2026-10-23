/*
 * A125925  Sprague-Grundy values for octal game .351.
 * https://oeis.org/A125925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [1,2,1,2,0,1,0,2] for n ≥ 1, repeated periodically with
 *            period 8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pat[8] = {
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)2,
        (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)2
    };
    if (n == 0) return (seq_t)0;          /* safety, not needed for the sequence */
    return pat[(n - 1) % 8];
}
