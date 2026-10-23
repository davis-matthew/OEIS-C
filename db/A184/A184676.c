/*
 * A184676  a(n) = n + floor((n/2-1/(4*n))^2); complement of A183867.
 * https://oeis.org/A184676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n/2 - 1/(4*n))^2) = n + floor((2*n*n - 1)^2 /
 *            (16*n*n))
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 240, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* Compute 2*nn*nn - 1 */
    seq_t t = nn * nn;          /* nn^2 */
    t = t * (seq_t)2;           /* 2*nn^2 */
    t = t - (seq_t)1;           /* 2*nn^2 - 1 */

    /* numerator of the fraction: (2*nn*nn - 1)^2 */
    seq_t num = t * t;

    /* denominator: 16*nn*nn */
    seq_t den = nn * nn;        /* nn^2 */
    den = den * (seq_t)16;      /* 16*nn^2 */

    return n + (num / den);
}
