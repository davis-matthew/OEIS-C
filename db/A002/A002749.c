/*
 * A002749  Sum of logarithmic numbers.
 * https://oeis.org/A002749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=0}^{n} d(n-k+1) * (-1)^k * n! / k! where d(m) is
 *            the number of divisors of m (A000005). Derived from E.g.f.:
 *            F(x)/exp(x)/x with F(x) = o.g.f. for divisors
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int count_divisors(idx_t n) {
    int count = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            count++;
            if (i * i != n) count++;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        // Compute n! / k! = product from i=0 to n-k-1 of (n-i)
        seq_t falling_factorial = 1;
        for (idx_t i = 0; i < n - k; i++) {
            falling_factorial *= (seq_t)(n - i);
        }
        
        idx_t m = n - k + 1;
        seq_t d_m = (seq_t)count_divisors(m);
        
        seq_t term = d_m * falling_factorial;
        
        if (k % 2 == 0) {
            result += term;
        } else {
            result -= term;
        }
    }
    
    return result;
}
