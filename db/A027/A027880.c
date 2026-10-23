/*
 * A027880  a(n) = Product_{i=1..n} (12^i - 1).
 * https://oeis.org/A027880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (12^i - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;
    seq_t pow12  = (seq_t)12;        /* 12^1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term   = pow12 - (seq_t)1;   /* (12^i - 1) */
        result = result * term;           /* multiply into product */
        pow12  = pow12 * (seq_t)12;       /* next power 12^(i+1) */
    }
    return result;
}
