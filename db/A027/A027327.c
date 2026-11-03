/*
 * A027327  a(n) = Sum_{k=0..m} (k+1) * A026120(n, m-k), where m=0 for n=0,1;
 *          m=n for n >= 2.
 * https://oeis.org/A027327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=12, a(n)=3*a(n-1) for n>2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"

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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    /* For n >= 2: a(n) = 12 * 3^(n-2) */
    seq_t val = pow3(n - 2);          /* compute 3^(n-2) */
    val = mul_small(val, 12);         /* multiply by 12 */
    return val;
}
