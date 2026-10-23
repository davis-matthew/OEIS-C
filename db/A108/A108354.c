/*
 * A108354  Expansion of 1/((1-x)^2(1+x^2)^2) in powers of x.
 * https://oeis.org/A108354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+4)/4 if n % 4 == 0 (n+3)/2 if n % 4 == 1 (n+2)/4 if n
 *            % 4 == 2 0 if n % 4 == 3 Expansion of 1/((1-x)^2(1+x^2)^2) in
 *            powers of x
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n & 3;  // n modulo 4
    if (m == 3) {
        return (seq_t)0;
    } else if (m == 1) {
        return ((seq_t)n + 3) / 2;
    } else if (m == 0) {
        return ((seq_t)n + 4) / 4;
    } else {  // m == 2
        return ((seq_t)n + 2) / 4;
    }
}
