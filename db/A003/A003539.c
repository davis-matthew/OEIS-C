/*
 * A003539  a(0) = 587, a(n) = 3*a(n-1) + 16 for n > 0 (the first 11 terms are
 *          primes).
 * https://oeis.org/A003539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 16, a(0) = 587 (equivalently a(n) = 595*3^n -
 *            8)
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = (seq_t)587;
    idx_t i;
    for (i = 0; i < n; i++) {
        v = (seq_t)3 * v + (seq_t)16;
    }
    return v;
}
