/*
 * A182460  a(n) = (3/5)*2^(4n+1) - (1/5).
 * https://oeis.org/A182460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/5)*2^(4*n+1) - (1/5) Recurrence: a(0) = 1; a(n) =
 *            16*a(n-1) + 3 (n > 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 1; /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 16 + 3;
    }
    return res;
}
