/*
 * A007923  Lengths increase by 1, digits cycle through positive digits.
 * https://oeis.org/A007923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{i=1..n} ((n*(n-1)/2+i-1 mod 9)+1)*10^(n-i)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t offset = ((seq_t)n * ((seq_t)n - 1)) / 2;
    seq_t power = 1;
    
    for (idx_t i = n; i >= 1; i--) {
        seq_t digit = ((offset + i - 1) % 9) + 1;
        result += digit * power;
        power *= 10;
    }
    
    return result;
}
