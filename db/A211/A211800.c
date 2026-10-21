/*
 * A211800  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2*w^2 < x^2 + y^2.
 * https://oeis.org/A211800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ordered triples (w,x,y) with w,x,y in {1,…,n}
 *               such that 2*w^2 < x^2 + y^2
 *
 * This implementation counts the triples by iterating over all ordered pairs
 * (x,y) with x ≤ y, computing the maximal w that satisfies the inequality,
 * and adding the contribution for the pair.  The calculation uses only
 * integer arithmetic; no floating–point operations are employed.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but safe guard */

    idx_t N = n;
    /* Pre‑compute squares 1^2 … N^2 */
    idx_t *sq = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    if (!sq) return 0;       /* memory error – return 0 as fallback */
    for (idx_t i = 1; i <= N; ++i) sq[i] = i * i;

    seq_t ans = 0;
    for (idx_t x = 1; x <= N; ++x) {
        idx_t wmax = 0;          /* current maximum w for this x */
        for (idx_t y = x; y <= N; ++y) {
            idx_t S = sq[x] + sq[y];   /* x^2 + y^2 */

            /* increase wmax while 2*(wmax+1)^2 < S */
            while (2 * (wmax + 1) * (wmax + 1) < S) {
                ++wmax;
            }

            seq_t add = (x == y) ? (seq_t)wmax : (seq_t)2 * wmax;
            ans += add;
        }
    }

    free(sq);
    return ans;
}
