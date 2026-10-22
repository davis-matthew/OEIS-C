/*
 * A005358  Number of low discrepancy sequences in base 5.
 * https://oeis.org/A005358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (deg(p_i) - 1), where p_1, p_2, ... are the
 *            monic irreducible polynomials over GF(5) ordered by degree
 *            (Niederreiter's t-value). There are I(d) = (1/d) * Sum_{k|d}
 *            mu(k) * 5^(d/k) of them of degree d
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   a005358_irr, a005358_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005358_irr.h"
#include "../utils/a005358_mu.h"

seq_t a(idx_t n) {
  seq_t result = 0;
  uint64_t remaining = n;
  uint64_t d;
  for (d = 1; remaining > 0 && d < 27; d++) {
    uint64_t c = a005358_irr(d);
    uint64_t take = c < remaining ? c : remaining;
    result += (seq_t)take * (seq_t)(d - 1);
    remaining -= take;
  }
  return result;
}
