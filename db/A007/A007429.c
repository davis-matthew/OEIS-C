/*
 * A007429  Inverse Moebius transform applied twice to natural numbers.
 * https://oeis.org/A007429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Sum_{d|n} sigma(d), where sigma is the sum of divisors
 * Range:     u8 n <= 47, u16 n <= 6299, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 82 terms through n = 9478 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma(idx_t n) {
    seq_t result = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            result += (seq_t)i;
            if (i * i != n) {
                result += (seq_t)(n / i);
            }
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            result += sigma(d);
            if (d * d != n) {
                result += sigma(n / d);
            }
        }
    }
    
    return result;
}
