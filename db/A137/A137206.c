/*
 * A137206  First differences of A074323.
 * https://oeis.org/A137206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0 a(1) = 2 a(2) = –1 a(n) = 2*a(n-2) for n ≥ 3 Equivalent
 *            closed forms: a(2k) = –2^(k‑1) for k ≥ 1 (a(0)=0) a(2k+1) =
 *            2^(k+1) for k ≥ 0
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 60, i64 n <= 124,
 *            i128 n <= 252, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    if (n == 2u)
        return (seq_t)(-1);

    /* For odd n, a(2k+1) = 2^(k+1) */
    if ((n & 1ULL) != 0ULL) {
        idx_t k = n >> 1;                /* k = n/2 */
        return pow2(k + 1);
    }

    /* For even n >= 4, a(2k) = –2^(k‑1) */
    idx_t k = n >> 1;                    /* k = n/2 */
    seq_t v = pow2(k - 1);               /* 2^(k-1) */
    return -v;
}
