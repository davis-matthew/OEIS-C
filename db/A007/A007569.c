/*
 * A007569  Number of nodes in regular n-gon with all diagonals drawn.
 * https://oeis.org/A007569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n for n <= 3; a(n) = binomial(n,4) + n for odd n; for n
 *            >= 4, a(n) = n + I(n), where I(n) is the Poonen-Rubinstein count
 *            of interior intersection points: I(n) = C(n,4) +
 *            (-5n^3+45n^2-70n+24)/24*d(2) - (3n/2)*d(4) +
 *            (-45n^2+262n)/6*d(6) + 42n*d(12) + 60n*d(18) + 35n*d(24) -
 *            38n*d(30) - 82n*...
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a7569_choose, a7569_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a7569_choose.h"
#include "../utils/a7569_gcd.h"

seq_t a(idx_t n) {
  seq_t pos, neg;
  seq_t N = (seq_t)n;
  if (n <= 3) return N;
  pos = a7569_choose(n, 4) + N;
  if (n % 2 == 1) return pos;
  neg = 0;
  {
    idx_t m = n / 2;
    pos += 1;
    pos += 5 * a7569_choose(m, 2);
    neg += 10 * a7569_choose(m, 3);
  }
  if (n % 4 == 0) neg += 3 * (seq_t)(n / 2);
  if (n % 6 == 0) {
    seq_t k = (seq_t)(n / 6);
    pos += 262 * k;
    neg += 270 * k * k;
  }
  if (n % 12 == 0) pos += 42 * N;
  if (n % 18 == 0) pos += 60 * N;
  if (n % 24 == 0) pos += 35 * N;
  if (n % 30 == 0) neg += 38 * N;
  if (n % 42 == 0) neg += 82 * N;
  if (n % 60 == 0) neg += 330 * N;
  if (n % 84 == 0) neg += 144 * N;
  if (n % 90 == 0) neg += 96 * N;
  if (n % 120 == 0) neg += 144 * N;
  if (n % 210 == 0) neg += 96 * N;
  return pos - neg;
}
