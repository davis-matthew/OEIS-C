/*
 * A007910  Expansion of 1/((1-2*x)*(1+x^2)).
 * https://oeis.org/A007910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(2n+1) = 2*a(2n), a(2n) = 2*a(2n-1) + (-1)^n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        v = v * 2;
        if ((i & 1) == 0) {
            if (((i / 2) & 1) == 0)
                v += 1;
            else
                v -= 1;
        }
    }
    return v;
}
