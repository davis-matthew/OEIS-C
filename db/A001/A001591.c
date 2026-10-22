/*
 * A001591  Pentanacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) +
 *          a(n-5), a(0)=a(1)=a(2)=a(3)=0, a(4)=1.
 * https://oeis.org/A001591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5),
 *            a(0)=a(1)=a(2)=a(3)=0, a(4)=1
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 70, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t w0 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 1;
  if (n < 4)
    return 0;
  for (idx_t i = 4; i < n; i++) {
    seq_t next = w0 + w1 + w2 + w3 + w4;
    w0 = w1;
    w1 = w2;
    w2 = w3;
    w3 = w4;
    w4 = next;
  }
  return w4;
}
