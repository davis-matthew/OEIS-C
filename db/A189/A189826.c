/*
 * A189826  a(n) = (3^n-n)*(n-1) - 2^n*(n-2).
 * https://oeis.org/A189826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - n)*(n-1) - 2^n*(n-2) (OEIS A189826)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute a(n) for n >= 1.
 * For n == 1 the direct formula involves a negative intermediate,
 * so handle this case separately.  For n >= 2 all terms are
 * non‑negative and the computation can be done in unsigned arithmetic. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* not used for the OEIS offset */
    if (n == 1) return 2;      /* a(1) = 2 */

    seq_t N = (seq_t)n;
    seq_t pow3_n = pow3(n);    /* 3^n */
    seq_t pow2_n = pow2(n);    /* 2^n */

    /* For n >= 2 the expression can be rearranged to avoid negative
     * intermediates:  a(n) = 3^n*(n-1) - n*(n-1) - 2^n*(n-2) */
    seq_t term = pow3_n * (N - 1) - N * (N - 1) - pow2_n * (N - 2);
    return term;
}
