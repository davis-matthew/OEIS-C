/*
 * A226003  Number of pairs of orthogonal (-5,7) vectors of length 12*n.
 * https://oeis.org/A226003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{k=0}^{n} C(12n, 7(n‑k)) * C(12n‑7(n‑k), 5k) *
 *            2^{5n+2k} (the sum ranges over all solutions of 5x + 7y = 35n,
 *            where x = 7(n‑k), y = 5k, and the remaining positions b = 12n –
 *            x – y = 5n + 2k.) See OEIS A226003
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* outside the offset */

    const idx_t L = 12 * n;                    /* total vector length */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t x = 7 * (n - k);                 /* positions where both are –5 */
        idx_t y = 5 * k;                       /* positions where both are  7 */
        idx_t b = 5 * n + 2 * k;               /* positions where they differ */

        /* term = C(L, x) * C(L – x, y) * 2^b   */
        seq_t term = binom(L, x);
        term = term * binom(L - x, y);
        term = term * pow_uint(2, b);

        sum += term;
    }

    /* unordered pairs: each pair counted twice in the ordered sum */
    sum /= 2;
    return sum;
}
