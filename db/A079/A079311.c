/*
 * A079311  Integer part of length of diagonal of integral rectangle with area
 *          n and smallest semiperimeter.
 * https://oeis.org/A079311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( A033676(n)^2 + A033677(n)^2 ) )
 * Range:     u8 n <= 16, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A033676(n) is the largest divisor of n not exceeding sqrt(n), */
/* and A033677(n) = n / A033676(n). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"

/* a(n): integer part of the diagonal of the integral rectangle with area n */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                /* area as seq_t */

    /* largest divisor d <= sqrt(n) */
    seq_t sqrt_n = isqrt_seq(n_seq);
    seq_t d = sqrt_n;
    for (; d > 0; --d) {
        if (n_seq % d == 0) break;
    }

    seq_t w = d;         /* width */
    seq_t h = n_seq / d; /* height */

    seq_t sum = w * w + h * h;            /* diagonal squared */
    return isqrt_seq(sum);        /* floor sqrt of the sum */
}
