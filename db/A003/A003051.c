/*
 * A003051  Number of inequivalent sublattices of index n in hexagonal
 *          lattice, where two sublattices are equivalent if they are related
 *          by a rotation or reflection preserving the hexagonal lattice.
 * https://oeis.org/A003051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{ d|n } (floor(d/6) + 1 - [d ≡ 2 or 6 (mod 12)] + [d
 *            ≡ 4 (mod 12)])
 * Range:     u8 n <= 356, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_contribution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_contribution.h"

seq_t a(idx_t n) {
  seq_t result = (seq_t)0;
  
  for (idx_t d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      result += divisor_contribution(d);
      
      idx_t d2 = n / d;
      if (d != d2) {
        result += divisor_contribution(d2);
      }
    }
  }
  
  return result;
}
