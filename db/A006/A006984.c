/*
 * A006984  Greatest minimal norm of sublattice of index n in hexagonal
 *          lattice.
 * https://oeis.org/A006984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: a(n) = max over all index-n sublattices (Hermite normal
 *            form basis (a,0),(b,d), a*d=n, 0<=b<a) of the minimal norm of
 *            x^2+xy+y^2 on the sublattice, found by Gauss reduction of the
 *            binary quadratic form
 * Range:     u8 n <= 255, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   fdiv_ll, min_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fdiv_ll.h"
#include "../utils/min_norm.h"

seq_t a(idx_t n) {
  long long N = (long long)n;
  long long best = 0;
  for (long long a1 = 1; a1 <= N; a1++) {
    if (N % a1 != 0) continue;
    long long d = N / a1;
    for (long long b = 0; b < a1; b++) {
      long long A = a1 * a1;
      long long B = 2 * a1 * b + a1 * d;
      long long C = b * b + b * d + d * d;
      long long m = min_norm(A, B, C);
      if (m > best) best = m;
    }
  }
  return (seq_t)best;
}
