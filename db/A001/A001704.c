/*
 * A001704  a(n) = n concatenated with n + 1.
 * https://oeis.org/A001704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n concatenated with n + 1
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 61 terms through n = 9677 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10_helper(int k) {
    seq_t result = 1;
    for (int i = 0; i < k; i++) {
        result *= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n_plus_1 = nseq + 1;
    
    // Count digits in n+1
    int digits = 0;
    seq_t temp = n_plus_1;
    while (temp > 0) {
        digits++;
        temp /= 10;
    }
    
    // Concatenate: n * 10^digits + (n+1)
    return nseq * pow10_helper(digits) + n_plus_1;
}
