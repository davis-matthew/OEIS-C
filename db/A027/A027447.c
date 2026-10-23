/*
 * A027447  Triangle read by rows: cube of the lower triangular mean matrix.
 * https://oeis.org/A027447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For a(n) the (i,j)th entry of the triangle (row i, column j) is
 *            N_{i,j} = Σ_{p=j}^{i} Σ_{q=j}^{p} ( L_i / p ) * ( L_i / q )
 *            where L_i = lcm(1,2,…,i). This follows from the fact that the
 *            row i of A³ has entries V_{i,j} = Σ_{p=j}^{i} Σ_{q=j}^{p} 1/(i p
 *            q). Since L_i is divisible by every integer ≤...
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   _floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Determine the row i and column j of the n‑th triangle entry */
    idx_t i = (_floor_isqrt_u64(1 + 8 * n) - 1) / 2;
    if (i * (i + 1) / 2 < n) ++i;               /* adjust for exact root  */
    idx_t j = n - (i - 1) * i / 2;              /* 1‑based column index   */

    /* Compute L_i = lcm(1,…,i) */
    seq_t Li = 1;
    for (idx_t d = 2; d <= i; ++d) {
        seq_t g = gcd(Li, (seq_t)d);
        Li = (Li / g) * (seq_t)d;               /* Li = lcm(Li,d)       */
    }

    /* Compute the numerator N_{i,j} */
    seq_t total = 0;
    seq_t cum   = 0;                            /* running sum of Li/p   */
    for (idx_t p = j; p <= i; ++p) {
        seq_t val = Li / (seq_t)p;               /* Li/p integer         */
        cum += val;                              /* Σ_{q=j}^{p} Li/q     */
        total += val * cum;                      /* add Li/p * Σ Li/q    */
    }

    return total;
}
