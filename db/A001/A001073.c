/*
 * A001073  Label a 1-cm ruler with digits 1 cm wide.
 * https://oeis.org/A001073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = the digit at position n of the concatenated labels
 *            L(0),L(1),..., where L(0)=0 and L(k+1) = L(k) + (number of
 *            decimal digits of L(k)) (A088235); label x covers positions
 *            x..x+len(x)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  idx_t x = 0;
  for (;;) {
    idx_t len = 1;
    idx_t t = x;
    while (t >= 10) {
      t /= 10;
      len++;
    }
    if (n < x + len) {
      idx_t k = len - 1 - (n - x);
      idx_t v = x;
      while (k > 0) {
        v /= 10;
        k--;
      }
      return (seq_t)(v % 10);
    }
    x += len;
  }
}
