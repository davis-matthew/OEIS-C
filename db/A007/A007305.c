/*
 * A007305  Numerators of Farey (or Stern-Brocot) tree fractions.
 * https://oeis.org/A007305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabf, nice, look
 * Formula:   for n >= 2 write n-1 = 2^m + p with 0 <= p < 2^m; start at L =
 *            0/1, R = 1/1 and read the m bits of p from most significant to
 *            least. For each bit take M = mediant(L,R): a 0 bit sets R = M, a
 *            1 bit sets L = M. Then a(n) = numerator of mediant(L,R). a(0) =
 *            0, a(1) = 1
 * Range:     u8 n <= 4096, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    idx_t k = n - 1;
    int m = 0;
    while ((k >> (m + 1)) != 0) m++;
    idx_t p = k - ((idx_t)1 << m);
    seq_t ln = 0, ld = 1, rn = 1, rd = 1;
    for (int b = m - 1; b >= 0; b--) {
        seq_t mn = ln + rn, md = ld + rd;
        if ((p >> b) & 1) {
            ln = mn; ld = md;
        } else {
            rn = mn; rd = md;
        }
    }
    return ln + rn;
}
