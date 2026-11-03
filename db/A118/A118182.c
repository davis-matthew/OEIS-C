/*
 * A118182  Antidiagonal sums of triangle A118180: a(n) = Sum_{k=0..[n/2]}
 *          (3^k)^(n-2*k) for n>=0.
 * https://oeis.org/A118182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (3^k)^(n-2*k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 17, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 118 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    // sum of terms
    seq_t sum = 0;
    // current power of 3: 3^k
    seq_t pow3k = 1;

    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t exp = n - 2 * k;          // exponent for this term
        seq_t term = ipow_i32(pow3k, (int)exp); // compute (3^k)^(n-2k)
        sum += term;                    // accumulate

        // prepare pow3k for next iteration: multiply by 3
        pow3k = mul_small(pow3k, 3);
    }
    return sum;
}
