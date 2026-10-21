/*
 * A221912  Partial sums of floor(n/12).
 * https://oeis.org/A221912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = sum_{k=0}^n floor(k/12) = q * (6*(q-1) + r + 1), where q
 *            = n / 12 and r = n % 12
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t q = n / 12;
    if (q == 0) {
        return (seq_t)0;
    }
    idx_t r = n % 12;
    seq_t q_ts = (seq_t)q;
    seq_t part = (seq_t)6 * ((seq_t)q - 1) + (seq_t)(r + 1);
    return q_ts * part;
}
