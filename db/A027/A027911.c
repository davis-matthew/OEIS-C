/*
 * A027911  a(n) = T(2*n+1,n), with T given by A027907.
 * https://oeis.org/A027911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+1,k)*binomial(2*n+1-k,
 *            n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* 2*n+1 fits into idx_t because n is uint64_t */
    idx_t two_n_plus_one = 2 * n + 1;
    idx_t limit = n >> 1;                 /* floor(n/2) */

    seq_t ans  = 0;
    seq_t C1   = 1;                       /* binomial(2n+1,0) */
    seq_t C2   = binom(two_n_plus_one, n); /* binomial(2n+1, n) */

    for (idx_t k = 0; k <= limit; ++k) {
        ans += C1 * C2;

        if (k == limit) break;           /* last term */

        /* Update C1: C(2n+1, k+1) = C(2n+1, k) * (2n+1-k)/(k+1) */
        C1 = (C1 * (seq_t)(two_n_plus_one - k)) / (seq_t)(k + 1);

        /* Update C2:
           C(2n+1-(k+1), n-2(k+1)) =
           C(2n+1-k, n-2k) * (n-2k)*(n-2k-1) /
           ((2n+1-k)*(2n+1-k-(n-2k)+1))
           Simplified denominator = (2n+1-k)*(2n+1-k-(n-2k)+1) = (2n+1-k)*(n-k+1)
         */
        idx_t r   = n - 2 * k;
        seq_t n2  = (seq_t)(two_n_plus_one - k);        /* current 2n+1-k */
        C2 = (C2 * (seq_t)r * (seq_t)(r - 1)) /
             (n2 * (n2 - (seq_t)r + 1));
    }

    return ans;
}
