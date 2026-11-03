/*
 * A211526  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n/2) + 2^(n+1) - 8 for n even a(n) = 2*(2^n +
 *            3*2^((n+1)/2) - 4) for n odd (OEIS A211526, offset 1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 69 (2026-11-01)
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
    /* The sequence is defined for n >= 1.  For n == 0 return 0
       (value never used). */
    if (n == 0)
        return 0;

    seq_t result;

    if (n & 1) {                /* n is odd */
        seq_t two_n   = pow2(n);                    /* 2^n            */
        seq_t two_half = pow2((n + 1) / 2);          /* 2^((n+1)/2)    */
        seq_t tmp = mul_small(two_half, 3);           /* 3*2^((n+1)/2)  */
        tmp = two_n + tmp;                           /* 2^n + 3*2^((n+1)/2) */
        tmp = mul_small(tmp, 2);                    /* 2*(...)         */
        result = tmp - 8;                            /* subtract 8      */
    } else {                      /* n is even */
        seq_t two_half = pow2(n / 2);                /* 2^(n/2)         */
        seq_t tmp = mul_small(two_half, 9);          /* 9*2^(n/2)       */
        seq_t two_n1 = pow2(n + 1);                  /* 2^(n+1)         */
        tmp = tmp + two_n1;                          /* 9*2^(n/2)+2^(n+1) */
        result = tmp - 8;                            /* subtract 8      */
    }

    return result;
}
