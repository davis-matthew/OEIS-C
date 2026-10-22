/*
 * A007732  Period of decimal representation of 1/n.
 * https://oeis.org/A007732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   multiplicative order of 10 modulo n (after removing factors of 2
 *            and 5)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    while (n % 2 == 0) n /= 2;
    while (n % 5 == 0) n /= 5;
    
    if (n == 1) return 1;
    
    idx_t power = 10 % n;
    idx_t m = 1;
    while (power != 1) {
        power = (power * 10) % n;
        m++;
    }
    
    return (seq_t)m;
}
