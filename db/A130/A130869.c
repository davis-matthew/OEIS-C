/*
 * A130869  Partial sums of A130752.
 * https://oeis.org/A130869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A130869: Partial sums of A130752.
   Formula: a(n) = 2^(n+2) – c[n mod 6],  where c = [2,1,0,0,1,2].
   This follows from the generating function (2‑x)/[(1‑x)(1‑2x)(1‑x+x²)]
   and the periodic pattern in the first differences. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* periodic correction values:  c[0] = 2, c[1] = 1, c[2] = 0,
       c[3] = 0, c[4] = 1, c[5] = 2 */
    static const seq_t c[6] = { (seq_t)2, (seq_t)1, (seq_t)0,
                                (seq_t)0, (seq_t)1, (seq_t)2 };
    idx_t mod = n % 6;
    return pow2((idx_t)(n + 2)) - c[mod];
}
