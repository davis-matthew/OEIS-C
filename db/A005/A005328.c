/*
 * A005328  Certain subgraphs of a directed graph.
 * https://oeis.org/A005328
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-2} (-1)^j * (n-1-j) * Product_{i=j+1..n-1}
 *            (2^i - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"


seq_t a(idx_t n) {
  seq_t even_sum = 0, odd_sum = 0;
  seq_t product = 1;
  
  for (idx_t i = n - 1; i > 0; i--) {
    seq_t factor = A001832_pow2(i) - 1;
    product *= factor;
    
    idx_t j = i - 1;
    seq_t coeff = (seq_t)(n - 1 - j);
    seq_t term = coeff * product;
    
    if (j % 2 == 0) {
      even_sum += term;
    } else {
      odd_sum += term;
    }
  }
  
  return even_sum - odd_sum;
}
