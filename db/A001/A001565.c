/*
 * A001565  3rd differences of factorial numbers.
 * https://oeis.org/A001565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 3*n^2 + 5*n + 2)*n!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t poly = ((sn + 3) * sn + 5) * sn + 2;
    seq_t f = 1;
    idx_t i;
    for (i = 2; i <= n; i++)
        f *= (seq_t)i;
    return poly * f;
}
