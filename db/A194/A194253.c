/*
 * A194253  [sum{(rk) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=sqrt(12).
 * https://oeis.org/A194253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt(12) * n(n+1)/2 ) - \ * sum_{k=1}^{n} floor(
 *            sqrt(12) * k )
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from a(n) = floor( sum_{k=1}^n { (sqrt(12)*k) } )  
 * and the identity {x} = x - floor(x).                     */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard just in case */

    /* M = n(n+1)/2  */
    idx_t M = n * (n + 1) / 2;

    /* Sum of floor(sqrt(12*k*k)) for k = 1..n */
    seq_t sum_floor = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* floor(sqrt(12)*k) = floor( sqrt(12*k*k) ) */
        idx_t x = 12 * k * k;          /* 12*k^2 fits in 64 bits for n≤10^4   */
        idx_t floor_k = isqrt(x);   /* exact integer square root           */
        sum_floor += (seq_t)floor_k;   /* promote to seq_t for the sum        */
    }

    /* A = floor(sqrt(12) * M) = floor( sqrt(12*M*M) ) */
    idx_t A = isqrt(12 * M * M);

    return (seq_t)A - sum_floor;
}
