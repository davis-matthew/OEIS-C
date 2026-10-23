/*
 * A186428  a(n) = sigma(n^2) modulo sigma(n).
 * https://oeis.org/A186428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n^2) modulo sigma(n)
 * Range:     u8 n <= 11, u16 n <= 816, u32 n <= 9553, u64 n <= 9553,
 *            u128 n <= 9553, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* n is at least 1 */
    idx_t sq = (idx_t)n * (idx_t)n;           /* n^2 fits in idx_t for n <= 10000 */
    seq_t sigma_n  = sigma_of(n);             /* sigma(n) */
    seq_t sigma_sq = sigma_of(sq);            /* sigma(n^2) */
    return sigma_sq % sigma_n;                /* sigma(n^2) mod sigma(n) */
}
