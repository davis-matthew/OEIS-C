/*
 * A052704  Apart from the leading term, a(n) = Catalan(n-1)*4^(n-1).
 * https://oeis.org/A052704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n-1) * 4^(n-1) for n≥1, a(0)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t m = n - 1;                    /* m = n-1 */

    /* Catalan(m) */
    seq_t cat = catalan(m);             /* C_m */

    /* 4^m, computed iteratively to avoid external pow helper */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < m; ++i) {
        pow4 *= (seq_t)4;
    }

    return cat * pow4;                  /* a(n) = Catalan(n‑1) * 4^(n‑1) */
}
