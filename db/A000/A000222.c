/*
 * A000222  a(n) = A058057(n,n-2).
 * https://oeis.org/A000222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} (-1)^k * (n-k)! * binomial(k,2) *
 *            binomial(2*n-k,k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t pos = 0, neg = 0;
    seq_t fact = 1;                 /* (n-k)! */
    bool small = (n <= 40);         /* binomial(2n-k,k) fits in uint64 */
    unsigned __int128 bs = 1;       /* binomial(2n-k,k), native path */
    seq_t bb = 1;                   /* binomial(2n-k,k), seq_t path */

    for (idx_t k = n;; k--) {
        seq_t B = small ? (seq_t)(uint64_t)bs : bb;
        seq_t t = fact * (seq_t)(k * (k - 1) / 2) * B;
        if (k % 2 == 0) pos += t; else neg += t;
        if (k == 2) break;

        /* step from k to k-1 */
        idx_t kk = k - 1;
        idx_t m = 2 * n - kk;
        fact *= (seq_t)(n - kk);
        /* C(m,kk) = C(m-1,kk+1) * m * (kk+1) / ((m-kk) * (m-kk-1)) */
        if (small) {
            bs = bs * m * (kk + 1) /
                 ((unsigned __int128)(m - kk) * (unsigned __int128)(m - kk - 1));
        } else {
            bb = bb * (seq_t)m * (seq_t)(kk + 1) /
                 ((seq_t)(m - kk) * (seq_t)(m - kk - 1));
        }
    }
    return pos - neg;
}
