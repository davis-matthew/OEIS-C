/*
 * A045899  Numbers k such that k+1 and 3*k+1 are perfect squares.
 * https://oeis.org/A045899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 8 with a(1)=0, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 850 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

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
    /* Handle small indices directly */
    if (n <= 1) return (seq_t)0;
    if (n == 2) return (seq_t)8;

    seq_t prev = (seq_t)0;   /* a(1) */
    seq_t cur  = (seq_t)8;   /* a(2) */
    seq_t nxt;

    for (idx_t i = 3; i <= n; ++i) {
        nxt = mul_small(cur, 14); /* 14 * a(n-1) */
        nxt -= prev;              /* subtract a(n-2) */
        nxt += (seq_t)8;           /* add constant 8 */
        prev = cur;
        cur = nxt;
    }
    return cur;
}
