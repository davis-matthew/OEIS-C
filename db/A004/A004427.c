/*
 * A004427  Arithmetic mean of digits of n (rounded up).
 * https://oeis.org/A004427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceiling(A007953(n)/A055642(n)), where A007953 is sum of
 *            digits and A055642 is count of digits
 * Range:     u8 n <= 25000, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 124 terms through n = 9531 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    
    seq_t sum = 0;
    seq_t digit_count = 0;
    idx_t temp = n;
    
    while (temp > 0) {
        sum += temp % 10;
        digit_count++;
        temp /= 10;
    }
    
    return (sum + digit_count - 1) / digit_count;
}
