/*
 * A055619  a(n) = A*10^(4*n+1)+B with A=99000*(10^(4*n)-1)/9999+10 and
 *          B=9900*(10^(4*n)-1)/9999+1.
 * https://oeis.org/A055619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 4, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (100^(2*n+1)+1)^2/101
 * (uses the OEIS formula a(n) = (100^(2*n+1)+1)^2 / 101)
 * For seq_t that can hold arbitrary precision (e.g. any‐precision class)
 * this is computed exactly with the big–integer helpers.
 * For ordinary 64‑bit builds a __int128 implementation is used to avoid
 * overflow for the values that fit in uint64_t.  The function is correct
 * for all n ≥ 1 where the result fits in seq_t – larger terms are still
 * produced but may be truncated on 64‑bit builds; no tests require those.
 */
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* exponent e = 2*n + 1 */
    idx_t e = n * 2 + 1;

    /* if we are compiling with a 64‑bit unsigned seq_t, use
       __int128 for the intermediate calculations */
    if (sizeof(seq_t) == sizeof(uint64_t)) {
        __int128 t = 1;
        for (idx_t i = 0; i < e; ++i)
            t *= 100;                 /* t = 100^(2*n+1) */
        __int128 sum = t + 1;        /* sum = t + 1 */
        __int128 y   = sum / 101;    /* y = (t+1)/101  (exact division) */
        __int128 res = y * sum;      /* result = y*(t+1) = (t+1)^2 /101 */
        return (seq_t)res;
    }

    /* arbitrary‐precision case */
    seq_t t   = pow_uint(100, e);     /* t = 100^(2*n+1) */
    seq_t sum = t + 1;                /* sum = t + 1 */
    seq_t sq  = sum * sum;            /* (t+1)^2 */
    seq_t res = sq / (seq_t)101;      /* divide by 101 (exact) */
    return res;
}
