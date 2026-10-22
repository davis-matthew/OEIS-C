/*
 * A002965  Interleave denominators (A000129) and numerators (A001333) of
 *          convergents to sqrt(2).
 * https://oeis.org/A002965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(2k) = q(k), a(2k+1) = p(k), where (q(0),p(0)) = (0,1) and
 *            q(k+1) = q(k) + p(k), p(k+1) = p(k) + 2*q(k) (equivalently a(n)
 *            = a(n-1) + a(n-2-(n mod 2)), a(0)=0, a(1)=1)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 52, u64 n <= 103,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q = 0, p = 1;
    idx_t k = n / 2;
    for (idx_t i = 0; i < k; i++) {
        seq_t nq = q + p;
        seq_t np = p + q + q;
        q = nq;
        p = np;
    }
    return (n % 2 == 0) ? q : p;
}
