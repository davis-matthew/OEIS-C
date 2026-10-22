/*
 * A003543  Divisors of 2^36 - 1.
 * https://oeis.org/A003543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^36 - 1, sorted
 * Range:     u8 n <= 3, u16 n <= 3, u32 none, u64 n <= 512, u128 n <= 512,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq.h"

seq_t a(idx_t n) {
  static seq_t divisors[512];
  static int computed = 0;
  
  if (!computed) {
    seq_t N = ((seq_t)1 << 36) - 1;
    int count = 0;
    
    for (seq_t d = 1; d * d <= N; d++) {
      if (N % d == 0) {
        divisors[count++] = d;
        if (d * d != N) {
          divisors[count++] = N / d;
        }
      }
    }
    
    qsort(divisors, count, sizeof(seq_t), cmp_seq);
    computed = 1;
  }
  
  return divisors[n - 1];
}
