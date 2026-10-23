/*
 * A174551  Triangular array T(n,k): functions f:{1,2,...,n}-> {1,2,...,n}
 *          such that each of k fixed (but arbitrary) elements are in the
 *          image of f.
 * https://oeis.org/A174551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2) and c = n -
 *            r(r+1)/2 T(n,k) = Σ_{i=0..k} (-1)^i * C(k,i) * (n-i)^n (OEIS
 *            A174551)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   binom, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Utility functions from the build */
#include "../utils/binom.h"      /* binom(idx_t a, idx_t b) -> seq_t */
#include "../utils/mul_small.h"  /* mul_small(seq_t x, unsigned int m) -> seq_t */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t a, seq_t b) -> seq_t */

/* local helper: compute base^exp (base is small, exp ≤ 200) */
static seq_t power_small(idx_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res = mul_small(res, (unsigned int)base);
    return res;
}

/* Main sequence generation function */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t c = n - r * (r + 1) / 2;

    seq_t pos = 0;  /* even i terms (additive)   */
    seq_t neg = 0;  /* odd  i terms (subtractive) */

    for (idx_t i = 0; i <= c; ++i)
    {
        seq_t bin = binom(c, i);          /* C(c,i)          */
        seq_t pwr = power_small(r - i, r);/* (r-i)^r         */
        seq_t term = seq_mul(bin, pwr);   /* bin * (r-i)^r   */

        if (i & 1)
            neg += term;
        else
            pos += term;
    }

    /* pos ≥ neg guaranteed for this combinatorial sum */
    return pos - neg;
}
