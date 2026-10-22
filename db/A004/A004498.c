/*
 * A004498  Tersum n + 9.
 * https://oeis.org/A004498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Tersum(n, 9) where Tersum is base-3 addition with no
 *            carries (mod 3)
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t result = 0;
  seq_t power_of_3 = 1;
  idx_t nine = 9;
  
  while (n > 0 || nine > 0) {
    idx_t digit_n = n % 3;
    idx_t digit_9 = nine % 3;
    seq_t digit_sum = ((seq_t)digit_n + (seq_t)digit_9) % 3;
    result += digit_sum * power_of_3;
    n /= 3;
    nine /= 3;
    power_of_3 *= 3;
  }
  
  return result;
}
