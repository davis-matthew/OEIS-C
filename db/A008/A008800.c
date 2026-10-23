/*
 * A008800  Molien series for group [2,7]+ = 227.
 * https://oeis.org/A008800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+2)*(n+8) - 7*(2*n+3)*(n mod 2) + 4*((5*n^2 + n + 3)
 *            mod 7)) / 28
 * Range:     u8 n <= 11, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t first = (ns + (seq_t)2) * (ns + (seq_t)8);
    seq_t temp = ns * (seq_t)2 + (seq_t)3;
    seq_t second = (seq_t)7 * temp * (ns & (seq_t)1);
    seq_t mod_part = (((seq_t)5 * ns * ns + ns + (seq_t)3) % (seq_t)7);
    seq_t third = (seq_t)4 * mod_part;
    seq_t numerator = first - second + third;
    return numerator / (seq_t)28;
}
