/*
 * A176642  Triangle T(n, k) = 8^(k*(n-k)), read by rows.
 * https://oeis.org/A176642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = 8^((c)*(r-c)), where r is the row index such that
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 and c = n - r*(r+1)/2
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t t = r * (r + 1) / 2;
    idx_t c = n - t;
    idx_t e = c * (r - c);

    seq_t res = (seq_t)1;
    seq_t base = (seq_t)8;
    while (e > 0) {
        if (e & 1) {
            res *= base;
        }
        base *= base;
        e >>= 1;
    }
    return res;
}
