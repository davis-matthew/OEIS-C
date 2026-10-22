/*
 * A000541  Sum of 7th powers: 1^7 + 2^7 + ... + n^7.
 * https://oeis.org/A000541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T^2*(6*T^2 - 4*T + 1)/3 with T = A000217(n) = n*(n+1)/2,
 *            i.e. a(n) = 2*A000217(n)^4 - (4/3)*A000217(n)^3 +
 *            (1/3)*A000217(n)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 20, u64 n <= 331, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t T;
    if (n % 2 == 0)
        T = (m / 2) * (m + 1);
    else
        T = m * ((m + 1) / 2);

    seq_t q = 6 * T * T - 4 * T + 1;

    /* T mod 3 is 0 or 1; if T == 1 (mod 3) then q == 0 (mod 3) */
    if (T % 3 == 0)
        return (T / 3) * T * q;
    else
        return T * T * (q / 3);
}
