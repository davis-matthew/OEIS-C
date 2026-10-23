/*
 * A010168  Continued fraction for sqrt(97).
 * https://oeis.org/A010168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=9; for n>0 a(n)=period[(n-1)%11] where
 *            period={1,5,1,1,1,1,1,1,5,1,18}
 * Range:     u8 n <= 8102, u16 n <= 8102, u32 n <= 8102, u64 n <= 8102,
 *            u128 n <= 8102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)9;
    static const seq_t period[11] = { (seq_t)1, (seq_t)5, (seq_t)1, (seq_t)1,
                                      (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
                                      (seq_t)5, (seq_t)1, (seq_t)18 };
    idx_t k = (n - 1) % 11;
    return period[k];
}
