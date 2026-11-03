/*
 * A005889  Theta series of hexagonal close-packing with respect to triangle
 *          between octahedra.
 * https://oeis.org/A005889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k in Z} c(n - 2*k^2), where c(m) = 3*(d_{1,3}(m) -
 *            d_{2,3}(m)) if m == 1 (mod 3) and c(m) = 0 otherwise; d_{i,3}(m)
 *            = number of divisors of m congruent to i mod 3. This is the norm
 *            count of a deep-hole coset of the hexagonal lattice, summed over
 *            the close-packed layers, which lie at s...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static long long a005889_c(idx_t m) {
  long long c1 = 0, c2 = 0;
  idx_t d;
  if (m == 0 || m % 3 != 1) return 0;
  for (d = 1; d * d <= m; d++) {
    if (m % d == 0) {
      idx_t e = m / d;
      if (d % 3 == 1) c1++;
      else if (d % 3 == 2) c2++;
      if (e != d) {
        if (e % 3 == 1) c1++;
        else if (e % 3 == 2) c2++;
      }
    }
  }
  return 3 * (c1 - c2);
}

seq_t a(idx_t n) {
  seq_t s = 0;
  idx_t k;
  s += (seq_t)a005889_c(n);
  for (k = 1; 2 * k * k < n; k++) {
    s += (seq_t)(2 * a005889_c(n - 2 * k * k));
  }
  return s;
}
