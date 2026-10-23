/*
 * A133883  a(n) = binomial(n+3,n) mod 3^2.
 * https://oeis.org/A133883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3, 3) mod 9 (i.e., C(n+3,3) mod 3^2)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nx = (seq_t)n;
    seq_t val = (nx + 3) * (nx + 2) * (nx + 1); /* numerator of binomial */
    val /= 6;                                     /* exact division */
    return val % 9;                                /* modulo 3^2 */
}
