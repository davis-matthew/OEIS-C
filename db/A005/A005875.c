/*
 * A005875  Theta series of simple cubic lattice; also number of ways of
 *          writing a nonnegative integer n as a sum of 3 squares (zero being
 *          allowed).
 * https://oeis.org/A005875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   r_3(n) = number of ordered triples (i, j, k) of integers such
 *            that i^2 + j^2 + k^2 = n
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt(idx_t x) {
    if (x == 0) return 0;
    idx_t y = x;
    idx_t z = (y + 1) / 2;
    while (z < y) {
        y = z;
        z = (z + x / z) / 2;
    }
    return y;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t sqrt_n = isqrt(n);
    
    for (idx_t i = 0; i <= sqrt_n; i++) {
        idx_t i_sq = i * i;
        if (i_sq > n) break;
        
        idx_t sqrt_ri = isqrt(n - i_sq);
        
        for (idx_t j = 0; j <= sqrt_ri; j++) {
            idx_t j_sq = j * j;
            if (i_sq + j_sq > n) break;
            
            idx_t remainder = n - i_sq - j_sq;
            idx_t k = isqrt(remainder);
            
            if (k * k == remainder) {
                int count_i = (i == 0) ? 1 : 2;
                int count_j = (j == 0) ? 1 : 2;
                int count_k = (k == 0) ? 1 : 2;
                
                count += (seq_t)(count_i * count_j * count_k);
            }
        }
    }
    
    return count;
}
