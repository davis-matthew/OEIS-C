/*
 * A040258  Continued fraction for sqrt(275).
 * https://oeis.org/A040258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   periodic continued fraction of sqrt(275); a(0) = 16, for n >= 1,
 *            a(n) = {1,1,2,1,1,32}[(n-1) mod 6]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cycle[6] = {
        (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)32
    };

    if (n == (idx_t)0) {
        return (seq_t)16;
    }
    idx_t idx = (n - (idx_t)1) % (idx_t)6;
    return cycle[idx];
}
