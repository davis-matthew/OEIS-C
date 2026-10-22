/*
 * A003621  Number of iterations until n reaches 1 or 4 under x goes to sum of
 *          squares of digits map.
 * https://oeis.org/A003621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if n = 1 or 4, else a(n) = 1 + a(A003132(n)) where
 *            A003132(n) is the sum of squares of digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t memo[10000];
    static unsigned char computed[10000];
    
    if (n == 1 || n == 4) return 0;
    
    if (n < 10000 && computed[n]) {
        return memo[n];
    }
    
    seq_t sum = 0;
    idx_t temp = n;
    while (temp > 0) {
        idx_t digit = temp % 10;
        sum += (seq_t)digit * digit;
        temp /= 10;
    }
    
    seq_t result = 1 + a((idx_t)sum);
    
    if (n < 10000) {
        memo[n] = result;
        computed[n] = 1;
    }
    
    return result;
}
