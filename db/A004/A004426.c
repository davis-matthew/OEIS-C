/*
 * A004426  Arithmetic mean of digits of n (rounded down).
 * https://oeis.org/A004426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(A007953(n)/A055642(n))
 * Range:     u8 n <= 777, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t digit_sum = 0;
    seq_t digit_count = 0;
    seq_t temp = (seq_t)n;
    
    do {
        digit_sum += temp % 10;
        digit_count++;
        temp /= 10;
    } while (temp > 0);
    
    return digit_sum / digit_count;
}
