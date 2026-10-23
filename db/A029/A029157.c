/*
 * A029157  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3+63*n^2+702*n+3240)/4536 * - (n+2)*(n^2 mod
 *            3)/27 * + ((3*n^3+3*n+2) mod 7)/7)
 * Range:     u8 none, u16 n <= 20, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;

    seq_t t1 = 2 * n3 + 63 * n2 + 702 * ns + 3240;

    seq_t n2mod3 = n2 % 3;
    seq_t term2 = 168 * (ns + 2) * n2mod3;          // 4536/27 = 168

    seq_t temp = (3 * n3 + 3 * ns + 2) % 7;
    seq_t term3 = 648 * temp;                        // 4536/7 = 648

    seq_t numer = t1 - term2 + term3;
    return numer / 4536;
}
