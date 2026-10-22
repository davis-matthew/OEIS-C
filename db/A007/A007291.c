/*
 * A007291  Series expansion for rectilinear polymers on square lattice.
 * https://oeis.org/A007291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(11*n^2 - 13*n + 3) / 6
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    seq_t n_minus_1 = n_s - 1;
    seq_t quad = 11 * n_s * n_s - 13 * n_s + 3;

    seq_t result;
    if (n_s % 2 == 0) {
        seq_t half = n_s / 2;
        if (half % 3 == 0) {
            result = (half / 3) * n_minus_1 * quad;
        } else if (n_minus_1 % 3 == 0) {
            result = half * (n_minus_1 / 3) * quad;
        } else {
            result = half * n_minus_1 * (quad / 3);
        }
    } else {
        seq_t half = n_minus_1 / 2;
        if (n_s % 3 == 0) {
            result = (n_s / 3) * half * quad;
        } else if (half % 3 == 0) {
            result = n_s * (half / 3) * quad;
        } else {
            result = n_s * half * (quad / 3);
        }
    }

    return result;
}
