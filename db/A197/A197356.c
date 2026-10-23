/*
 * A197356  The number of labeled directed graphs (with self loops allowed) on
 *          n nodes of at most two colors, where no edge connects nodes of
 *          distinct colors.
 * https://oeis.org/A197356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) * Sum_{k=0..n} C(n,k) * 4^(k*(k-n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             = Sum_{k=0..n} C(n,k) * 2^{(n‑k)^2 + k^2}   (same result) */
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t sum   = 0;
    seq_t comb  = 1;           /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t diff = n - k;                             /* n - k */
        idx_t exp  = diff * diff + k * k;               /* (n-k)^2 + k^2 */
        seq_t part = pow2(exp);                         /* 2^exp */
        sum += comb * part;                             /* add term */

        if (k == n) break;                             /* last iteration */
        /* binomial recurrence: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        comb = comb * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
