/*
 * A028059  Expansion of 1/((1-3x)(1-5x)(1-7x)(1-8x)).
 * https://oeis.org/A028059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^(n+4)-15*7^(n+3)+2*5^(n+4)-3^(n+4))/120
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"

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
    /* Convert exponents to seq_t */
    seq_t exp8 = (seq_t)(n + 4);
    seq_t exp7 = (seq_t)(n + 3);
    seq_t exp5 = exp8;                 /* same as n+4 */
    seq_t exp3 = exp8;                 /* same as n+4 */

    /* Compute powers */
    seq_t p8  = ipow((seq_t)8,  exp8);
    seq_t p7  = ipow((seq_t)7,  exp7);
    seq_t p5  = ipow((seq_t)5,  exp5);
    seq_t p3  = ipow((seq_t)3,  exp3);

    /* Combine terms with exact integer arithmetic */
    seq_t term = p8;
    term -= mul_small(p7, 15);
    term += mul_small(p5, 2);
    term -= p3;

    /* Exact division by 120 */
    return term / (seq_t)120;
}
