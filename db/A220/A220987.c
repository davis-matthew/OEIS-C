/*
 * A220987  The left Aurifeuillian factor of 11^(22n+11) + 1.
 * https://oeis.org/A220987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 161051 z^10 - 161051 z^9 + 73205 z^8 - 14641 z^7 - 1331
 *            z^6 + 1331 z^5 - 121 z^4 - 121 z^3 + 55 z^2 - 11 z + 1 (with z =
 *            11^n)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute z = 11^n */
    seq_t z = 1;
    for (idx_t i = 0; i < n; ++i) {
        z *= 11;
    }
    seq_t res = 161051;
    res = res * z - 161051;   /* 161051 z^10 - 161051 z^9 */
    res = res * z + 73205;    /* + 73205 z^8 */
    res = res * z - 14641;    /* - 14641 z^7 */
    res = res * z - 1331;     /* - 1331 z^6 */
    res = res * z + 1331;     /* + 1331 z^5 */
    res = res * z - 121;      /* - 121 z^4 */
    res = res * z - 121;      /* - 121 z^3 */
    res = res * z + 55;       /* + 55 z^2 */
    res = res * z - 11;       /* - 11 z */
    res = res * z + 1;        /* + 1 */
    return res;
}
