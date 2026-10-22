/*
 * A004500  Tersum n + 11.
 * https://oeis.org/A004500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = tersum(n, 11): convert n and 11 to base 3, add
 *            digit-by-digit mod 3
 * Range:     u8 n <= 245, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    int base3_11[3] = {2, 0, 1};  // 11 in base 3, least significant digit first
    int idx = 0;
    
    while (n > 0 || idx < 3) {
        int digit_n = (int)(n % 3);
        int digit_11 = (idx < 3) ? base3_11[idx] : 0;
        int sum = (digit_n + digit_11) % 3;
        result += (seq_t)sum * power;
        power *= 3;
        n /= 3;
        idx++;
    }
    
    return result;
}
