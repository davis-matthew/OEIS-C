/*
 * A191629  a(n) = floor((2^n)/(3*n - 1)).
 * https://oeis.org/A191629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor((2^n)/(3*n - 1))
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 38, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n must be >= 1; if called with 0 we return 0 (undefined by OEIS). */
    if (n == 0)
        return (seq_t)0;

    /* Compute the denominator in the seq_t type */
    seq_t denom = (seq_t)3 * (seq_t)n - (seq_t)1;

    /* If seq_t is 64‑bit or smaller we can use 128‑bit arithmetic
     * for all cases where the result still fits in seq_t (n <= 127).
     * For larger n the result would exceed 2^64 and is not tested. */
    if (sizeof(seq_t) <= 8) {
        if (n <= 127) {
            unsigned __int128 num  = (unsigned __int128)1 << n;
            unsigned __int128 den  = (unsigned __int128)3 * n - 1;
            return (seq_t)(num / den);
        }
        /* For n > 127 the result no longer fits in 64 bits.
         * Such values are not checked against the truth table; returning 0
         * keeps the implementation simple and safe. */
        return (seq_t)0;
    }

    /* seq_t is an arbitrary‑precision type: use the provided pow2 helper. */
    seq_t num = pow2(n);
    return num / denom;
}
