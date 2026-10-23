/*
 * A017964  Powers of sqrt(20) rounded down.
 * https://oeis.org/A017964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(20^n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 21,
 *            big any n
 * Helpers:   floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the small‑integer (≤64‑bit) case separately to avoid
       overflow of the intermediate power.  For larger seq_t types
       (arbitrary precision) we just delegate to the generic
       utilities. */
    if (sizeof(seq_t) <= 8) {
        /* Compute 20^k in 128‑bit intermediate to keep the value
           exact for all n where the result fits in 64 bits. */
        uint64_t k = n / 2;          /* floor(n/2) */
        __int128 pow = 1;
        if ((n & 1) == 0) {
            /* n even – result is exactly 20^k */
            for (uint64_t i = 0; i < k; ++i)
                pow *= 20;
            return (seq_t)pow;
        } else {
            /* n odd – we need floor(20^k * sqrt(20)) */
            for (uint64_t i = 0; i < k; ++i)
                pow *= 20;          /* m = 20^k */
            /* Compute floor(sqrt(20 * m^2)) with integer arithmetic. */
            __int128 rhs = (__int128)20 * pow * pow;  /* 20 * m^2 */
            __int128 low = 0;
            __int128 high = (__int128)1 << 64;        /* large enough */
            while (low < high) {
                __int128 mid = (low + high + 1) >> 1;
                if (mid * mid <= rhs)
                    low = mid;
                else
                    high = mid - 1;
            }
            return (seq_t)low;
        }
    } else {
        /* seq_t is an arbitrary‑precision integer type: use the
           generic utilities. */
        seq_t val = seq_pow((seq_t)20, n);
        return floor_sqrt(val);
    }
}
