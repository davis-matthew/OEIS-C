/*
 * A087487  a(n) = 4*(2^(n^2-n+1)*(n^2-n)-n+3).
 * https://oeis.org/A087487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(2^(n^2-n+1)*(n^2-n)-n+3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for basic integer types */
#include "../utils/pow2seq.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* undefined for n==0, return 0 to avoid UB */

    seq_t nn          = (seq_t)n;
    seq_t n_sq        = nn * nn;                /* n^2 */
    seq_t n_sq_minus_n = n_sq - nn;             /* n^2 - n */

    /* exponent: n^2 - n + 1, cast to idx_t (fits in 64‑bit for practical n) */
    idx_t exp = (idx_t)(n_sq_minus_n + 1);

    seq_t pow2 = pow2seq(exp);                  /* 2^(n^2-n+1) */

    seq_t prod = pow2 * n_sq_minus_n;           /* 2^(n^2-n+1)*(n^2-n) */
    seq_t tmp  = prod - nn + 3;                 /* minus n plus 3 */

    return 4 * tmp;                             /* multiply by 4 */
}
