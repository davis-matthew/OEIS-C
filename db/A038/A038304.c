/*
 * A038304  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*2^j.
 * https://oeis.org/A038304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n)=binomial(i,j)*10^(i-j)*2^j where
 *            i=floor((sqrt(8*n+1)-1)/2), j=n-i(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
 * Helpers:   binomial, ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* compute i */
    seq_t t = 8 * (seq_t)n + 1;
    seq_t s = floor_sqrt(t);
    idx_t i = (idx_t)((s - 1) / 2);

    /* compute j */
    idx_t tri = i * (i + 1) / 2;
    idx_t j = n - tri;

    idx_t diff = i - j;          /* i - j = exponent of 10 */

    /* value = binomial(i,j) * 10^(i-j) * 2^j */
    seq_t val = binomial(i, j);
    val = val * pow10(diff);
    val = val * pow2(j);

    return val;
}
