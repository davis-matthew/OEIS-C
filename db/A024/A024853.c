/*
 * A024853  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = (natural numbers >= 2).
 * https://oeis.org/A024853
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(m+1)*(3*n+5-2*m)/6 where m = floor(n/2)
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = n / 2;
    seq_t result = m * (m + 1);
    result *= ((seq_t)3 * n + 5 - 2 * m);
    result /= 6;
    return result;
}
