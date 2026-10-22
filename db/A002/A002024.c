/*
 * A002024  k appears k times; a(n) = floor(sqrt(2n) + 1/2).
 * https://oeis.org/A002024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = ceiling((sqrt(1 + 8*n) - 1) / 2), equivalently the unique
 *            k such that k*(k+1) >= 2*n
 * Range:     u8 n <= 145, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t n_val = (seq_t)n;
  seq_t target = 2 * n_val;
  
  // Exponential search for an upper bound
  seq_t hi = 1;
  while (hi * (hi + 1) < target) {
    hi *= 2;
  }
  
  // Binary search for the smallest k such that k*(k+1) >= target
  seq_t lo = hi / 2;
  while (lo < hi) {
    seq_t mid = lo + (hi - lo) / 2;
    if (mid * (mid + 1) < target) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }
  
  return lo;
}
