/*
 * A137387  Triangular sequence from coefficients of the expansion of
 *          p(x,t)=Exp[2*x*t]*t/(1 - t).
 * https://oeis.org/A137387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((k)! * 2^m) / m! , where k = r - 1 and r is the row
 *            number (r >= 1) such that 1 + k*(k+1)/2 >= n. Then m = n - (1 +
 *            (k-1)*k/2) - 1. The sequence is read row‑major from the
 *            triangular array
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   ipow_u64_idx, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The entry with n = 1 is the singleton {0}.  For n >= 2 the above
   formulas apply. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/isqrt.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 1)
        return (seq_t)0;

    /* Find the smallest k such that k*(k+1)/2 >= n-1  (n>=2) */
    idx_t t = n - 1;                       /* elements after the first */
    idx_t disc = 1 + 8 * t;                /* 1 + 8*t */
    idx_t s = isqrt(disc);       /* floor sqrt */
    if (s * s < disc)                     /* make it ceil sqrt */
        ++s;
    idx_t k = (s - 1) / 2;                /* floor part of formula */
    /* Ensure k is the minimal integer with k*(k+1)/2 >= t */
    while (k * (k + 1) / 2 < t)
        ++k;

    /* r = k + 1 is the row number */
    idx_t r = k + 1;

    /* Number of elements before row r */
    idx_t prev_total = 1 + (k - 1) * k / 2; /* 1 + T_{k-1} */

    /* Offset within the row (0‑based) */
    idx_t m = n - prev_total - 1;          /* 0 <= m < k */

    /* Compute (k)! / m! as product of integers from m+1 to k */
    seq_t val = seq_one();                 /* start with 1 */
    for (idx_t i = m + 1; i <= k; ++i) {
        val = mul_small(val, (unsigned int)i);
    }

    /* Multiply by 2^m */
    seq_t pow2m = ipow_u64_idx(2, m);
    val = seq_mul(val, pow2m);

    return val;
}
