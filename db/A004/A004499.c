/*
 * A004499  Tersum n + 10.
 * https://oeis.org/A004499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = tersum(n, 10), where tersum is base-3 addition with no
 *            carries (mod 3)
 * Range:     u8 n <= 245, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t result = 0;
  seq_t power_of_3 = 1;
  seq_t temp_n = (seq_t)n;
  seq_t temp_10 = 10;
  
  while (temp_n > 0 || temp_10 > 0) {
    seq_t digit_n = temp_n % 3;
    seq_t digit_10 = temp_10 % 3;
    seq_t sum_digit = (digit_n + digit_10) % 3;
    
    result += sum_digit * power_of_3;
    
    temp_n /= 3;
    temp_10 /= 3;
    power_of_3 *= 3;
  }
  
  return result;
}
