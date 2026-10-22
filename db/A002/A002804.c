/*
 * A002804  (Presumed) solution to Waring's problem: g(n) = 2^n +
 *          floor((3/2)^n) - 2.
 * https://oeis.org/A002804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + floor((3/2)^n) - 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* invariant: 3^k = q*2^k + r, 0 <= r < P = 2^k */
    seq_t q = 1, r = 0, P = 1;
    idx_t k;
    for (k = 0; k < n; k++) {
        seq_t t = q * 3;
        seq_t b = t % 2;
        seq_t x = b * P + r * 3;   /* < 4*2^k */
        seq_t P2 = P * 2;
        q = t / 2;
        if (x >= P2) {
            q += 1;
            x -= P2;
        }
        r = x;
        P = P2;
    }
    return P + q - 2;
}
