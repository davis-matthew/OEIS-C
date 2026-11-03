/*
 * A026391  Expansion of 1/((1-2x)(1-6x)(1-7x)(1-10x)).
 * https://oeis.org/A026391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2^n/20 + 27*6^n/2 - 343*7^n/15 + 125*10^n/12
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

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
    /* Compute the needed powers. */
    seq_t p2  = ipow_idx((seq_t)2,  n);
    seq_t p6  = ipow_idx((seq_t)6,  n);
    seq_t p7  = ipow_idx((seq_t)7,  n);
    seq_t p10 = ipow_idx((seq_t)10, n);

    /*  Common denominator is 60.  Compute each contribution. */
    seq_t t1 = mul_small(p2,  3);    /* 3*2^n  */
    seq_t t2 = mul_small(p6, 810);   /* 810*6^n */
    seq_t t3 = mul_small(p7, 1372);  /* 1372*7^n */
    seq_t t4 = mul_small(p10, 625);  /* 625*10^n */

    /*  numerator = t2 + t4 - (t1 + t3) */
    seq_t num = t2 + t4 - (t1 + t3);

    return num / (seq_t)60;
}
