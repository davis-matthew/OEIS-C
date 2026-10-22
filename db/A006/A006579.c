/*
 * A006579  a(n) = Sum_{k=1..n-1} gcd(n,k).
 * https://oeis.org/A006579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d divides n} (d-1)*phi(n/d)
 * Range:     u8 n <= 59, u16 n <= 3779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_phi(seq_t n) {
    if (n == 1) return 1;
    
    seq_t result = n;
    seq_t temp = n;
    
    if (temp % 2 == 0) {
        while (temp % 2 == 0) temp /= 2;
        result -= result / 2;
    }
    
    for (seq_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            while (temp % i == 0) temp /= i;
            result -= result / i;
        }
    }
    
    if (temp > 1) {
        result -= result / temp;
    }
    
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            seq_t phi_n_d = compute_phi((seq_t)(n / d));
            result += ((seq_t)d - 1) * phi_n_d;
            
            if (d != n / d) {
                idx_t other_d = n / d;
                seq_t phi_d = compute_phi((seq_t)d);
                result += ((seq_t)other_d - 1) * phi_d;
            }
        }
    }
    
    return result;
}
