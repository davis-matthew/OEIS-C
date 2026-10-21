/*
 * A228635  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length 9.
 * https://oeis.org/A228635
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ( Σ_{r=1}^{⌊n/9⌋} (n-1)^{r-1} * C(n-9r+r-1, r-1) -
 *            Σ_{r=1}^{⌊n/10⌋} (n-1)^{r-1} * C(n-10r+r-1, r-1) ) where C(a,b)
 *            is the binomial coefficient. This counts words of length n over
 *            an alphabet of size n whose minimal run length equals exactly 9
 * Range:     u8 n <= 17, u16 n <= 28, u32 n <= 47, u64 n <= 80, u128 n <= 143,
 *            big any n
 * Helpers:   comb_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/comb_idx.h"

seq_t a(idx_t n) {
    if (n < 9) return 0;          /* below the sequence offset */
    seq_t n_seq = (seq_t)n;
    seq_t res9 = 0, res10 = 0;

    for (int step = 9; step <= 10; ++step) {
        seq_t sum = 0;
        seq_t pow = 1;            /* (n-1)^{r-1}  for current r */
        idx_t max_r = n / step;
        for (idx_t r = 1; r <= max_r; ++r) {
            idx_t N = n - step * r + r - 1;
            idx_t K = r - 1;
            seq_t bin = comb_idx(N, K);
            sum += pow * bin;
            pow *= (seq_t)(n - 1);
        }
        if (step == 9)
            res9 = n_seq * sum;
        else
            res10 = n_seq * sum;
    }
    return res9 - res10;
}
