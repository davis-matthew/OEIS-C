/*
 * A231149  Greatest integer k such that n+1 + ... + n+k <= 1 + ... + n.
 * https://oeis.org/A231149
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((-2*n - 1 + sqrt(8*n^2 + 8*n + 1)) / 2)
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 978 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A231149 */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t for safe arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Calculate discriminant D = 8*n^2 + 8*n + 1 */
    seq_t D = (seq_t)8 * n_seq * n_seq + (seq_t)8 * n_seq + (seq_t)1;

    /* Integer square root of D */
    seq_t sqrtD = floor_sqrt(D);

    /* Compute (sqrtD - (2*n + 1)) / 2 */
    seq_t diff = sqrtD - ((seq_t)2 * n_seq + (seq_t)1);
    seq_t result = diff / (seq_t)2;

    return result;
}
