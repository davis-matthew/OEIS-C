/*
 * A002409  a(n) = 2^n*C(n+6,6). Number of 6D hypercubes in an
 *          (n+6)-dimensional hypercube.
 * https://oeis.org/A002409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*C(n+6,6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 40, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= 6; i++) {
        r = r * ((seq_t)n + (seq_t)i);
        r = r / (seq_t)i;
    }
    for (i = 0; i < n; i++)
        r = r * 2;
    return r;
}
