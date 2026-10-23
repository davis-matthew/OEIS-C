/*
 * A158938  Numbers congruent to {-3,7,9,11,13,23,25,27} mod 64.
 * https://oeis.org/A158938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 64 * (n / 8) + offset[n % 8] where offset[0] = -3,
 *            offset[1] = 7, offset[2] = 9, offset[3] = 11, offset[4] = 13,
 *            offset[5] = 23, offset[6] = 25, offset[7] = 27
 * Range:     i8 n <= 16, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t offset[8] = {(seq_t)-3, (seq_t)7, (seq_t)9, (seq_t)11,
                                    (seq_t)13, (seq_t)23, (seq_t)25, (seq_t)27};
    idx_t q = n / 8;
    idx_t r = n % 8;
    return ((seq_t)q) * 64 + offset[r];
}
