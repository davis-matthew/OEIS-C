/*
 * A002819  Liouville's function L(n) = partial sums of A008836.
 * https://oeis.org/A002819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, sign, changed
 * Formula:   a(n) = Sum_{k=1..n} (-1)^Omega(k), where Omega(k) counts prime
 *            factors with multiplicity
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 0) return 0;
  
  seq_t result = 0;
  
  for (idx_t k = 1; k <= n; k++) {
    int omega = 0;
    idx_t temp = k;
    
    while (temp % 2 == 0) {
      omega++;
      temp /= 2;
    }
    
    for (idx_t p = 3; p * p <= temp; p += 2) {
      while (temp % p == 0) {
        omega++;
        temp /= p;
      }
    }
    
    if (temp > 1) {
      omega++;
    }
    
    seq_t lambda_k = ((omega & 1) == 0) ? 1 : -1;
    result += lambda_k;
  }
  
  return result;
}
