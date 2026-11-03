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
 * Validated: standard, pass, 89 terms through n = 960 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t divisor_contribution(idx_t d) {
  seq_t contrib = (seq_t)d / 6 + 1;
  idx_t d_mod = d % 12;
  if (d_mod == 2 || d_mod == 6) {
    contrib -= 1;
  }
  if (d_mod == 4) {
    contrib += 1;
  }
  return contrib;
}

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
