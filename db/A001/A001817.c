/*
 * A001817  G.f.: Sum_{n>0} x^n/(1-x^(3n)) = Sum_{n>=0} x^(3n+1)/(1-x^(3n+1)).
 * https://oeis.org/A001817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive divisors of n of the form 3k+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            if (d % 3 == 1) count++;
            if (d != n / d) {
                idx_t other = n / d;
                if (other % 3 == 1) count++;
            }
        }
    }
    return count;
}
