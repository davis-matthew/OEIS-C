/*
 * A005004  Davenport-Schinzel numbers of degree n on 3 symbols.
 * https://oeis.org/A005004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - 5 if n is odd, 3*n - 4 if n is even, for n >= 4;
 *            base cases a(1)=1, a(2)=3, a(3)=5
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 1) return 1;
  if (n == 2) return 3;
  if (n == 3) return 5;
  if (n == 4) return 8;
  
  seq_t n_seq = (seq_t)n;
  if (n % 2 == 0) {
    return 3 * n_seq - 4;
  } else {
    return 3 * n_seq - 5;
  }
}
