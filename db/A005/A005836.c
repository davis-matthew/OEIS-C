/*
 * A005836  Numbers whose base-3 representation contains no 2.
 * https://oeis.org/A005836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base, tabf
 * Formula:   a(n) = Sum_{k=0..m} b(k)*3^k where (n-1) = Sum_{k=0..m} b(k)*2^k
 * Range:     u8 n <= 39, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    n = n - 1;
    seq_t result = 0;
    seq_t power_of_3 = 1;
    
    while (n > 0) {
        if (n & 1) {
            result += power_of_3;
        }
        n >>= 1;
        power_of_3 *= 3;
    }
    
    return result;
}
