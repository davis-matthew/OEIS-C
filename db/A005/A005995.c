/*
 * A005995  Alkane (or paraffin) numbers l(8,n).
 * https://oeis.org/A005995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A038163(n) + 3*A038163(n-2), where A038163 has G.f.
 *            1/((1-x)^3*(1-x^2)^3), i.e. the third partial sum of b(2j) =
 *            (j+1)*(j+2)/2, b(2j+1) = 0; overall G.f.
 *            (1+3*x^2)/((1-x)^3*(1-x^2)^3)
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 249, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t be = 0;            /* b(2j) built incrementally */
  seq_t s1 = 0, s2 = 0, s3 = 0;
  seq_t q0 = 0, q1 = 0, q2 = 0; /* c(k), c(k-1), c(k-2) */
  idx_t k;
  for (k = 0; k <= n; k++) {
    seq_t bk = 0;
    if (k % 2 == 0) {
      be += (seq_t)(k / 2 + 1);
      bk = be;
    }
    s1 += bk;
    s2 += s1;
    s3 += s2;
    q2 = q1;
    q1 = q0;
    q0 = s3;
  }
  return q0 + 3 * q2;
}
