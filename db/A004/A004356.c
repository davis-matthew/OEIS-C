/*
 * A004356  Binomial coefficient C(6n,n-1).
 * https://oeis.org/A004356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    
    for (idx_t i = 1; i < n; i++) {
        seq_t num = (seq_t)(6 * n - i + 1);
        seq_t den = (seq_t)i;
        
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        
        g = gcd(result, den);
        result /= g;
        den /= g;
        
        result *= num;
        result /= den;
    }
    
    return result;
}
