/*
 * A133259  a(6n) = 6n+1, a(6n+1) = 6n+2, a(6n+2) = 6n+3, a(6n+3) = 6n+6,
 *          a(6n+4) = 6n+5, a(6n+5) = 6n+4.
 * https://oeis.org/A133259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n/6)*6) + f(n%6) where f(0)=1, f(1)=2, f(2)=3, f(3)=6,
 *            f(4)=5, f(5)=4
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 6;
    idx_t t = n / 6;
    static const seq_t add[6] = {1, 2, 3, 6, 5, 4};
    return ((seq_t)t * 6) + add[r];
}
