/*
 * A178686  Expansion of the polynomial (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9).
 * https://oeis.org/A178686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in the polynomial
 *            (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > (idx_t)24) return 0;
    seq_t coeff = 0;
    for (int mask = 0; mask < 16; ++mask) {
        idx_t sum = 0;
        if (mask & 1)  sum += (idx_t)3;
        if (mask & 2)  sum += (idx_t)5;
        if (mask & 4)  sum += (idx_t)7;
        if (mask & 8)  sum += (idx_t)9;
        if (sum == n) coeff++;
    }
    return coeff;
}
