/*
 * A005353  Number of 2 X 2 matrices with entries mod n and nonzero
 *          determinant.
 * https://oeis.org/A005353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^4 - A020478(n), where A020478(n) is the number of 2x2
 *            matrices mod n with zero determinant
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_gcd_divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_gcd_divisor_sum.h"

static uint64_t phi(uint64_t n) {
  uint64_t result = n;
  for (uint64_t p = 2; p * p <= n; p++) {
    if (n % p == 0) {
      while (n % p == 0) n /= p;
      result -= result / p;
    }
  }
  if (n > 1) result -= result / n;
  return result;
}

seq_t a(idx_t n) {
  if (n == 1) return 0;
  
  seq_t total = (seq_t)n * (seq_t)n * (seq_t)n * (seq_t)n;
  seq_t zero_det = 0;
  
  for (idx_t d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      {
        idx_t g = d;
        uint64_t count_a = phi(n / g);
        uint64_t sum_term = sum_gcd_divisor_sum(g);
        seq_t ng = (seq_t)(n / g);
        seq_t count_bc = ng * ng * (seq_t)sum_term;
        zero_det += (seq_t)count_a * (seq_t)g * count_bc;
      }
      
      if (d != n / d) {
        idx_t g = n / d;
        uint64_t count_a = phi(n / g);
        uint64_t sum_term = sum_gcd_divisor_sum(g);
        seq_t ng = (seq_t)(n / g);
        seq_t count_bc = ng * ng * (seq_t)sum_term;
        zero_det += (seq_t)count_a * (seq_t)g * count_bc;
      }
    }
  }
  
  return total - zero_det;
}
