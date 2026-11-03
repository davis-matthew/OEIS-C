/*
 * A136719  Number of labeled directed trees with n nodes.
 * https://oeis.org/A136719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * (3*n)^(n-2) for n>1; a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 180 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    if (n == 1) {
        return (seq_t)1;
    }
    /* base = 3 * n  */
    seq_t base = (seq_t)3 * (seq_t)n;
    /* exponent = n - 2  */
    seq_t exp  = (seq_t)(n - 2);
    /* pow = (3*n)^(n-2) */
    seq_t val  = ipow(base, exp);
    /* multiply by 3  */
    return mul_small(val, 3);
}
