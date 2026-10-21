/*
 * A226097  a(n) = ((-1)^n + 2*n - 38)*(2*n - 38) + 41.
 * https://oeis.org/A226097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n + 2*n - 38)*(2*n - 38) + 41
 * Range:     u8 none, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    seq_t val = (seq_t)4 * n_s * n_s;     /* 4*n^2 */
    if ((n & 1) == 0) {                  /* n even */
        val = val - (seq_t)150 * n_s + (seq_t)1447;
    } else {                             /* n odd */
        val = val - (seq_t)154 * n_s + (seq_t)1523;
    }
    return val;
}
