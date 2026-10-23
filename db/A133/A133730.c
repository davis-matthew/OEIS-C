/*
 * A133730  Alternating sign sequence A033999 interleaved with Jacobsthal
 *          sequence A001045.
 * https://oeis.org/A133730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n/2) if n even; = (2^(n/2) - (-1)^(n/2))/3 if n odd
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0ULL) {
        idx_t k = n >> 1;
        return ((k & 1ULL) == 0ULL) ? 1 : -1;
    } else {
        idx_t k = n >> 1;            /* k = floor(n/2) */
        seq_t pow2k = pow_uint(2ULL, k);
        seq_t sign   = ((k & 1ULL) == 0ULL) ? 1 : -1;   /* (-1)^k */
        seq_t diff   = pow2k - sign;
        return diff / 3;
    }
}
