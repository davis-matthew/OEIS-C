/*
 * A005431  Embeddings of n-bouquet in sphere.
 * https://oeis.org/A005431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2^n * (2*n-1)! / (n+1)!, for n > 0; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"


seq_t a(idx_t n) {
  if (n == 0) {
    return 1;
  }
  
  seq_t result = pw2(n);
  
  if (n == 1) {
    result /= 2;
  } else if (n > 2) {
    for (idx_t i = n + 2; i <= 2 * n - 1; i++) {
      result *= (seq_t)i;
    }
  }
  
  return result;
}
