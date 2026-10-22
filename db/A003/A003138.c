/*
 * A003138  Nearest integer to 24*(2^n - 1)/n.
 * https://oeis.org/A003138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(24*(2^n - 1)/n), ties rounded up. Long division of
 *            2^n - 1 (n one-bits) by n gives q, r; then 24*(2^n-1) = 24*q*n +
 *            24*r, so the remainder is carried in idx_t and only the quotient
 *            is kept in seq_t
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q = 0;
    idx_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++) {
        r = 2 * r + 1;
        q = q * 2;
        if (r >= n) {
            r -= n;
            q += 1;
        }
    }
    idx_t t = 24 * r;
    idx_t q2 = t / n;
    idx_t rem = t % n;
    seq_t res = q * 24 + (seq_t)q2;
    if (2 * rem >= n) res += 1;
    return res;
}
