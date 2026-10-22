/*
 * A001807  a(n) = n! * binomial(n,5).
 * https://oeis.org/A001807
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * binomial(n,5)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t i;
    for (i = 1; i <= 5; i++) {
        c = c * ((seq_t)(n - 5 + i)) / (seq_t)i;
    }
    seq_t f = 1;
    for (i = 2; i <= n; i++) {
        f *= (seq_t)i;
    }
    return f * c;
}
