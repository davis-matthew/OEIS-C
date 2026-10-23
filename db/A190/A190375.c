/*
 * A190375  a(n) = n + [n*r/u] + [n*s/u] + [n*t/u]; r=sin(Pi/5), s=1/r,
 *          t=sin(2*Pi/5), u=1/t.
 * https://oeis.org/A190375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*√5/4⌋ + ⌊n*φ⌋ + ⌊n*(5+√5)/8⌋
 * Range:     u8 n <= 7, u16 n <= 1825, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ = (1+√5)/2.  The floor terms are computed via integer arithmetic
   using only integer operations, avoiding floating point. */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* Integer square‑root via binary search.  Works for any seq_t that
   supports <, <=, +, -, *, /, <<. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;
    unsigned bl = bitlen_seq_t(x);
    high <<= (bl + 1) / 2;           /* high ≈ 2^(ceil(bitlen/2)) */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        seq_t sq  = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                  /* low is first value with low^2 > x */
}

seq_t a(idx_t n)
{
    seq_t nn       = (seq_t)n * (seq_t)n;   /* n^2                */
    seq_t val      = nn * 5;                /* 5*n^2              */
    seq_t sqrt_floor = floor_sqrt(val);     /* ⌊n√5⌋               */

    seq_t term1 = sqrt_floor / 4;                          /* ⌊n√5/4⌋          */
    seq_t term2 = ((seq_t)n + sqrt_floor) / 2;             /* ⌊nφ⌋              */
    seq_t term3 = (((seq_t)5 * n) + sqrt_floor) / 8;      /* ⌊n*(5+√5)/8⌋      */

    return (seq_t)n + term1 + term2 + term3;
}
