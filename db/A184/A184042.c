/*
 * A184042  1/9 the number of (n+1) X 4 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 3*2^(n/2) + 2^(n+1) + 17 , n even * 2^(n+1) +
 *            2^((n+3)/2) + 17 , n odd }
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 190 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

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
    /* n >= 1 ; behaviour for n < 1 unspecified */
    seq_t res;
    if ((n & 1) == 0) { /* even */
        seq_t t1 = pow2(n / 2);
        t1 = mul_small(t1, 3);
        seq_t t2 = pow2(n + 1);
        res = t1 + t2 + (seq_t)17;
    } else { /* odd */
        seq_t t1 = pow2(n + 1);
        seq_t t2 = pow2((n + 3) / 2);
        res = t1 + t2 + (seq_t)17;
    }
    return res;
}
