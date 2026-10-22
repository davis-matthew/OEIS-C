/*
 * A007886  Number of cycles induced by iterating the Gray-coding of an n-bit
 *          number: a(n+1) = a(n) + 2^n/C_n, where C_n = least power of 2 >= n
 *          (C_n is the length of the cycle), with a(0) = 1.
 * https://oeis.org/A007886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-L-1) + Sum_{k=0..L} 2^(2^k-k-1) where L =
 *            floor(log_2(n))
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    
    // Compute L = floor(log_2(n))
    idx_t L = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        L++;
    }
    
    // Compute sum from k=0 to L of 2^(2^k - k - 1)
    seq_t sum = 0;
    for (idx_t k = 0; k <= L; k++) {
        idx_t exp = ((idx_t)1 << k) - k - 1;
        sum += (seq_t)1 << exp;
    }
    
    // Compute 2^(n - L - 1) and add to sum
    return ((seq_t)1 << (n - L - 1)) + sum;
}
