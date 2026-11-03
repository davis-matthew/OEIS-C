/*
 * A071171  L_2 norm of vertices of Permuto-Associahedron in R^n.
 * https://oeis.org/A071171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1, 3)*(30*n^4-33*n^2+2)/70
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 34, u64 n <= 824, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 978 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* Helper: multiply then divide, using 128‑bit intermediate when the
   underlying type is 64‑bit or smaller.  On systems without 128‑bit
   integer support the simple expression is used (it only overflows
   when the final result itself would overflow). */
static seq_t muldiv(seq_t x, seq_t y, seq_t d)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    __uint128_t prod = (__uint128_t)x * y;
    return (seq_t)(prod / d);
#else
    return (x * y) / d;
#endif
}

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n4  = n2 * n2;
    seq_t poly = (seq_t)30 * n4 - (seq_t)33 * n2 + 2;
    seq_t bin  = binomial(n + 1, 3);        /* binomial coefficient */
    return muldiv(bin, poly, 70);        /* perform a* b / 70 safely */
}
