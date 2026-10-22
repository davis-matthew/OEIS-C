/*
 * A006103  Gaussian binomial coefficient [ 2n,n ] for q=3.
 * https://oeis.org/A006103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 3^(k^2) * ([n,k]_3)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t e) {
    if (e == 0) return 1;
    seq_t result = 1;
    seq_t base = 3;
    while (e > 0) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

static seq_t gauss_binom_3(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        seq_t num = pow3(n - i) - 1;
        seq_t den = pow3(i + 1) - 1;
        result = result * num / den;
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t gauss = gauss_binom_3(n, k);
        idx_t k2 = k * k;
        seq_t pow3_k2 = pow3(k2);
        seq_t term = pow3_k2 * gauss * gauss;
        result += term;
    }
    
    return result;
}
