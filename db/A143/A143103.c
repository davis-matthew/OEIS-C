/*
 * A143103  Row sums of triangle A143102.
 * https://oeis.org/A143103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} k·A(k) where A(k) is defined by A(1) = 1 A(k)
 *            = k+1 if k%3 == 0, A(k) = k-1 if k%3 == 1 (k>1), A(k) = k if k%3
 *            == 2
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t term;
        if (k == 1) {
            term = 1;
        } else {
            idx_t mod = k % 3;
            if (mod == 0) {
                term = kk * (kk + 1);        /* k*(k+1) = k^2 + k   */
            } else if (mod == 1) {
                term = kk * (kk - 1);        /* k*(k-1) = k^2 - k   */
            } else { /* mod == 2 */
                term = kk * kk;              /* k^2                 */
            }
        }
        sum += term;
    }
    return sum;
}
