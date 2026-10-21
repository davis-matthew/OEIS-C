/*
 * A052790  Expansion of e.g.f.: x^2*log(1-x)^4.
 * https://oeis.org/A052790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24 * n * (n-1) * |Stirling1(n-2,4)| for n >= 2 * (a(n) =
 *            0 for n < 2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The unsigned Stirling numbers of the first kind satisfy
     *   s(0,0) = 1, s(n,0) = 0 for n > 0, s(0,k) = 0 for k > 0
     *   s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k)
     *
     * We only need the column k = 4, so we keep a small DP array
     * of the current row for k=0..4.
     */
    static seq_t prev[5] = { (seq_t)1, 0, 0, 0, 0 };  /* s(0,k) */
    static idx_t max_computed = 0;                   /* largest n for which prev holds s(n,k) */

    /* The value we need is |Stirling1(n-2,4)|, so in terms of rows
     * we need to have computed up to target = n-2.
     */
    if (n < 2) {
        return 0;
    }

    idx_t target = n - 2;
    if (target > max_computed) {
        seq_t cur[5];
        for (idx_t k = 1; k <= 4; ++k) {
            cur[0] = 0; /* will be set to 0 for all k, but it's fine */
        }
        for (idx_t i = max_computed + 1; i <= target; ++i) {
            /* compute row for n = i */
            cur[0] = 0;                     /* s(i,0) = 0 for i > 0 */
            cur[1] = prev[0] + (seq_t)(i - 1) * prev[1];
            cur[2] = prev[1] + (seq_t)(i - 1) * prev[2];
            cur[3] = prev[2] + (seq_t)(i - 1) * prev[3];
            cur[4] = prev[3] + (seq_t)(i - 1) * prev[4];
            /* copy back to prev for next iteration */
            prev[0] = cur[0];
            prev[1] = cur[1];
            prev[2] = cur[2];
            prev[3] = cur[3];
            prev[4] = cur[4];
        }
        max_computed = target;
    }

    /* Now prev[4] holds |Stirling1(target,4)| = |Stirling1(n-2,4)| */
    const seq_t F24 = (seq_t)24;
    seq_t result = F24;
    result *= (seq_t)n;
    result *= (seq_t)(n - 1);
    result *= prev[4];
    return result;
}
