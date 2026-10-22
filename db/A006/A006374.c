/*
 * A006374  Number of positive definite reduced binary quadratic forms of
 *          discriminant -4*n.
 * https://oeis.org/A006374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 8398, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: count reduced forms [a,2*b,c] with b^2 - a*c = -n, |2*b| <= a <= c, and b >= 0 whenever 2*|b| = a or a = c. Each b > 0 with 2*b < a and a < c counts twice, for +b and -b. */

seq_t a(idx_t n) {
  seq_t count = 0;
  unsigned long long N = (unsigned long long)n;
  unsigned long long av, b;

  /* Reduced forms satisfy 3*a^2 <= 4*n */
  for (av = 1; 3 * av * av <= 4 * N; av++) {
    for (b = 0; 2 * b <= av; b++) {
      unsigned long long ca = b * b + N;
      unsigned long long c;
      if (ca % av != 0) continue;
      c = ca / av;
      if (c < av) continue;
      if (b == 0 || 2 * b == av || av == c)
        count += 1;
      else
        count += 2;
    }
  }
  return count;
}
