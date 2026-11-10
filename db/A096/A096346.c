/*
 * A096346  Complement of A004128.
 * https://oeis.org/A096346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n-th (0-indexed) positive integer not in A004128, where
 *            A004128(m) = Sum_{k>=0} floor(m/3^k) = m + v3(m!); computed by
 *            locating the smallest M with v3(M!) >= n+1, then a(n) =
 *            A004128(M) - (v3(M!) - n)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8988 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A096346_f(idx_t m) {
    seq_t result = 0;
    seq_t t = (seq_t)m;
    while (t > 0) {
        result += t;
        t /= 3;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t cnt = 0;
    idx_t M = 0;
    while (cnt < n + 1) {
        M++;
        idx_t t = M;
        while (t % 3 == 0) {
            cnt++;
            t /= 3;
        }
    }
    return A096346_f(M) - (seq_t)(cnt - n);
}
