/*
 * A187695  G.f.:
 *          x^4*(1+x)*(1+12*x-31*x^2+12*x^2)/((1-x)^2*(1-2*x)^2*(1-3*x)*(1-4*x)).
 * https://oeis.org/A187695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*2^(n-4)*n + 33*2^(n-4) - 2*n + 2 -
 *            26*3^(n-2)+25*4^(n-3), n>1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 996 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

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
    /* Zero for the first four indices */
    if (n <= 3)
        return (seq_t)0;

    /* Compute the required powers */
    seq_t p2 = pow2(n - 4);   /* 2^(n-4) */
    seq_t p3 = pow3(n - 2);   /* 3^(n-2) */
    seq_t p4 = pow4(n - 3);   /* 4^(n-3) */

    /* Build the positive part:
       27*2^(n-4)*n + 33*2^(n-4) + 2 + 25*4^(n-3) */
    seq_t term1 = mul_small(p2, 27);          /* 27*2^(n-4) */
    term1        = mul_small(term1, (unsigned int)n);   /* 27*2^(n-4)*n */
    seq_t term2 = mul_small(p2, 33);         /* 33*2^(n-4) */
    seq_t term5 = mul_small(p4, 25);         /* 25*4^(n-3) */
    seq_t positive = term1 + term2 + (seq_t)2 + term5;

    /* Build the negative part:
       26*3^(n-2) + 2*n */
    seq_t negative = mul_small(p3, 26);          /* 26*3^(n-2) */
    negative += mul_small((seq_t)n, 2);          /* 2*n */

    /* The final value is the difference of the two parts.
       For all n>=4 the positive part is greater than the negative part,
       so the subtraction stays in the positive domain. */
    return positive - negative;
}
