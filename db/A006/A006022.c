/*
 * A006022  Sprague-Grundy (or Nim) values for the game of Maundy cake on an n
 *          X 1 sheet.
 * https://oeis.org/A006022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = n*(1/p1 + 1/(p1*p2) + ... + 1/(p1*...*pk))
 *            where p1, p2, ..., pk are prime factors with repetition
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    idx_t temp = n;
    seq_t product = 1;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        while (temp % p == 0) {
            product *= (seq_t)p;
            result += (seq_t)n / product;
            temp /= p;
        }
    }
    
    if (temp > 1) {
        product *= (seq_t)temp;
        result += (seq_t)n / product;
    }
    
    return result;
}
