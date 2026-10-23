/*
 * A033725  Product theta3(q^d); d | 13.
 * https://oeis.org/A033725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m^2 + 13*k^2 = n} (m==0?1:2) * (k==0?1:2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt(idx_t x) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t max_m = isqrt(n);
    for (idx_t m = 0; m <= max_m; ++m) {
        idx_t sq_m = m * m;
        seq_t coeff_m = (sq_m == 0 ? 1 : 2);
        idx_t rem = n - sq_m;
        if (rem == 0) {
            res += coeff_m;
        } else if ((rem % 13) == 0) {
            idx_t sq_k = rem / 13;
            idx_t k = isqrt(sq_k);
            if (k * k == sq_k) {
                res += coeff_m * 2;
            }
        }
    }
    return res;
}
