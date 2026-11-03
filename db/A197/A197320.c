/*
 * A197320  a(n) = cosh(n*arccosh(2^n)).
 * https://oeis.org/A197320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cosh(n*arccosh(2^n)) = T_n(2^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where T_k(x) is the Chebyshev polynomial of the first kind,
   T_0(x)=1, T_1(x)=x, T_{k+1}(x)=2*x*T_k(x)-T_{k-1}(x).   */

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
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* x = 2^n  */
    seq_t x = pow2(n);

    /* 2*x is needed repeatedly in the recurrence */
    seq_t two_x = mul_small(x, (unsigned)2);

    seq_t t_prevprev = (seq_t)1;   /* T_0(x) */
    seq_t t_prev     = x;          /* T_1(x) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = two_x * t_prev;  /* 2*x*T_{i-1} */
        seq_t ti  = tmp - t_prevprev;/* T_i = 2*x*T_{i-1} - T_{i-2} */
        t_prevprev = t_prev;
        t_prev     = ti;
    }

    return t_prev; /* T_n(x) = a(n) */
}
