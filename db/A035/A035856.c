/*
 * A035856  Coordination sequence for A_30 lattice.
 * https://oeis.org/A035856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = Σ_{d=1}^{min(30,n)} C(31,d) * C(n-1,d-1) *
 *            C(30-d+n, n) for n ≥ 1, where C(a,b) is the binomial
 *            coefficient. The parameter n is related to the norm m by m =
 *            2*n. This implements the OEIS definition of A035856
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute C(31,d) once */
    static bool init = false;
    static seq_t C31[31];            /* indices 1..30 used */
    if (!init) {
        for (idx_t d = 1; d <= 30; ++d)
            C31[d] = binomial(31, d);
        init = true;
    }

    seq_t sum = 0;
    idx_t maxd = n < 30 ? (idx_t)n : 30;
    for (idx_t d = 1; d <= maxd; ++d) {
        seq_t term = C31[d];
        term *= binomial(n - 1, d - 1);
        term *= binomial(30 - d + n, n);
        sum += term;
    }
    return sum;
}
