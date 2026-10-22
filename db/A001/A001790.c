/*
 * A001790  Numerators in expansion of 1/sqrt(1-x).
 * https://oeis.org/A001790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = numerator(binomial(2*n,n)/4^n) =
 *            A000984(n)/2^(popcount(n)); recurrence a(n) = (a(n-1) /
 *            oddpart(n)) * (2*n-1), a(0) = 1 (oddpart(n) always divides
 *            a(n-1) exactly, so no intermediate exceeds a(n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        idx_t m = k;
        while ((m & 1) == 0) m >>= 1;
        r /= (seq_t)m;
        r *= (seq_t)(2 * k - 1);
    }
    return r;
}
