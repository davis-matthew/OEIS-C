/*
 * A002750  Sum of logarithmic numbers.
 * https://oeis.org/A002750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n+1} A000203(m)*binomial(n, m-1)*(m-1)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma(idx_t m) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            sum += (seq_t)d;
            if (d != m / d) {
                sum += (seq_t)(m / d);
            }
        }
    }
    return sum;
}

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result = result * (seq_t)(n - i) / (seq_t)(i + 1);
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t fact = 1;
    
    for (idx_t m = 1; m <= n + 1; m++) {
        seq_t sig = sigma(m);
        seq_t binom_coeff = binom(n, m - 1);
        result += sig * binom_coeff * fact;
        fact *= (seq_t)m;
    }
    return result;
}
