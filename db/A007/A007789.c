/*
 * A007789  a(n) = (1/n) * Sum_{x in (Z/nZ)^3} e^(2*Pi*i*det(circ(x))/n), the
 *          normalized Gauss sum of the 3 X 3 circulant determinant.
 * https://oeis.org/A007789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative with a(p^k) = p^(k-1)*((k+1)*p-k) if p ≡
 *            1 (mod 3), a(p^k) = p^(2*floor(k/2)) if p ≡ 2 (mod 3), a(3) = 0,
 *            a(3^k) = 3^(k+1) for k >= 2
 * Range:     u8 n <= 62, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Factorize n and apply multiplicative formula
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t k = 0;
            
            // Count the power of p
            while (temp % p == 0) {
                k++;
                temp /= p;
            }
            
            // Compute a(p^k)
            seq_t apk;
            if (p == 3) {
                if (k == 1) {
                    return 0;
                } else {
                    // a(3^k) = 3^(k+1) for k >= 2
                    apk = 1;
                    for (idx_t i = 0; i < k + 1; i++) {
                        apk *= (seq_t)3;
                    }
                }
            } else if (p % 3 == 1) {
                // a(p^k) = p^(k-1) * ((k+1)*p - k)
                seq_t pk_minus_1 = 1;
                for (idx_t i = 0; i < k - 1; i++) {
                    pk_minus_1 *= (seq_t)p;
                }
                seq_t factor = (seq_t)(k + 1) * (seq_t)p - (seq_t)k;
                apk = pk_minus_1 * factor;
            } else {
                // p % 3 == 2: a(p^k) = p^(2*floor(k/2))
                apk = 1;
                for (idx_t i = 0; i < 2 * (k / 2); i++) {
                    apk *= (seq_t)p;
                }
            }
            
            result *= apk;
        }
    }
    
    // Handle the remaining prime factor if temp > 1
    if (temp > 1) {
        idx_t p = temp;
        
        if (p == 3) {
            return 0;
        } else if (p % 3 == 1) {
            // a(p) = 2*p - 1
            result *= ((seq_t)2 * (seq_t)p - (seq_t)1);
        }
        // If p % 3 == 2, a(p) = 1, so no change to result
    }
    
    return result;
}
