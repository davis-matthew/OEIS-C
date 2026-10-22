/*
 * A001757  Expansion of an integral: central elements of rows of triangle in
 *          A059366.
 * https://oeis.org/A001757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000984(floor(n/2))*A000984(ceiling(n/2))*n!/2^n,
 *            computed via a(0) = 1, a(2m) = a(2m-1)*2*(2m-1), a(2m+1) =
 *            (a(2m)/(m+1))*(2m+1)^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            v *= (seq_t)2 * (seq_t)(i - 1);
        } else {
            idx_t m = i / 2;
            v /= (seq_t)(m + 1);
            v *= (seq_t)i;
            v *= (seq_t)i;
        }
    }
    return v;
}
