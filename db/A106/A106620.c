/*
 * A106620  a(n) = numerator of n/(n+19).
 * https://oeis.org/A106620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = numerator of n/(n+19) = n / gcd(n,19)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Since 19 is prime, gcd(n,19) = 19 if n%19==0 else 1 */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t g = (n % 19 == 0) ? 19 : 1;
    return (seq_t)(n / g);
}
