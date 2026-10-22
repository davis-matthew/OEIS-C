/*
 * A001525  a(n) = (3n)!/(3!n!).
 * https://oeis.org/A001525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3n)! / (3! * n!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   gcd_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_helper.h"

seq_t a(idx_t n) {
  seq_t result = 1;
  seq_t divisor = 6;
  
  for (idx_t k = n + 1; k <= 3 * n; k++) {
    result = result * (seq_t)k;
    
    if (divisor > 1) {
      seq_t g = gcd_helper(result, divisor);
      result /= g;
      divisor /= g;
    }
  }
  
  if (divisor > 1) {
    result /= divisor;
  }
  
  return result;
}
