/*
 * A006977  Cellular automaton with Rule 230: 000, 001, 010, 011, ..., 111 ->
 *          0,1,1,0,0,1,1,1.
 * https://oeis.org/A006977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-4) - 4*a(n-6), a(0)=1, a(1)=3, a(2)=5,
 *            a(3)=15, a(4)=23, a(5)=59
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t a_vals[6] = {1, 3, 5, 15, 23, 59};
  
  if (n < 6) {
    return a_vals[n];
  }
  
  for (idx_t i = 6; i <= n; i++) {
    seq_t next = 4 * a_vals[(i - 2) % 6] + a_vals[(i - 4) % 6] - 4 * a_vals[(i - 6) % 6];
    a_vals[i % 6] = next;
  }
  
  return a_vals[n % 6];
}
