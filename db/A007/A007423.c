/*
 * A007423  a(n) = mu(n) + 1, where mu is the Moebius function.
 * https://oeis.org/A007423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = mu(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 2;
    
    idx_t temp = n;
    int prime_count = 0;
    
    if (temp % 2 == 0) {
        prime_count++;
        temp /= 2;
        if (temp % 2 == 0) return 1;
    }
    
    for (idx_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            prime_count++;
            temp /= i;
            if (temp % i == 0) return 1;
        }
    }
    
    if (temp > 1) prime_count++;
    
    return (prime_count % 2 == 0) ? 2 : 0;
}
