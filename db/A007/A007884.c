/*
 * A007884  Chimes made by clock striking quarter-hours.
 * https://oeis.org/A007884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Periodic sequence with period 48 describing clock chimes for a
 *            12-hour clock. * Each 4-term cycle (quarter-hours in an hour)
 *            yields: 1 (quarter past), 2 (half past), 3 (quarter to), h (on
 *            the hour), * where h cycles from 1 to 12
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  idx_t remainder = n % 4;
  
  if (remainder < 3) {
    return (seq_t)(remainder + 1);
  }
  return (seq_t)((n / 4) % 12 + 1);
}
