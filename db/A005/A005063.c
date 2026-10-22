/*
 * A005063  Sum of squares of primes dividing n.
 * https://oeis.org/A005063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p^2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    if (temp % 2 == 0) {
        result += 4;
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    for (idx_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            result += (seq_t)i * i;
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    
    if (temp > 1) {
        result += (seq_t)temp * temp;
    }
    
    return result;
}
