/*
 * A003793  Order of (usually) simple Chevalley group A_n (3).
 * https://oeis.org/A003793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n(n+1)/2) * ∏_{k=1}^{n} (3^(k+1) - 1) / gcd(n+1, 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    
    // Compute 3^(n(n+1)/2)
    idx_t exp = ((idx_t)n * ((idx_t)n + 1)) / 2;
    seq_t result = pow_seq((seq_t)3, exp);
    
    // Compute ∏_{k=1}^{n} (3^(k+1) - 1)
    seq_t pow3 = 9; // 3^2
    for (idx_t k = 1; k <= n; k++) {
        seq_t term = pow3 - 1;
        result *= term;
        pow3 *= 3;
    }
    
    // Divide by gcd(n+1, 2)
    // gcd(n+1, 2) = 2 if n is odd, 1 if n is even
    if (n & 1) {
        result /= 2;
    }
    
    return result;
}
