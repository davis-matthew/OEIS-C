/*
 * A054335  A convolution triangle of numbers based on A000984 (central
 *          binomial coefficients of even order).
 * https://oeis.org/A054335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 152, u64 n <= 561,
 *            u128 n <= 2082, big any n
 * Validated: standard, pass, 71 terms through n = 4943 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A054335: Flattened convolution triangle based on central binomial coefficients.
   The triangle entry T(n,m) satisfies the vertical recurrence
       T(n,m) = 2*(2*n - m - 1) * T(n-1,m) / (n - m)   (n > m ≥ 0)
       T(m,m) = 1.
   a(k) is the flattened value: the first element of row 0, then row 1,
   then row 2, etc.  For k we find the row r and column c such that
   r*(r+1)/2 ≤ k < (r+1)*(r+2)/2, and compute T(r,c) by iterating
   the recurrence from m=c up to m=r.  The computation uses an
   integer‑arithmetic safe multiplication/division that employs
   128‑bit intermediates when seq_t is 64 bits, avoiding overflow
   for all n that fit in seq_t. */

#include <stdint.h>
#include <stdbool.h>

/* Safe multiplication/division:  x * num / den
   Uses 128‑bit intermediate on 64‑bit seq_t, otherwise ordinary arithmetic. */
static inline seq_t safe_mul_div(seq_t x, seq_t num, seq_t den)
{
#ifdef __SIZEOF_INT128__
    /* When seq_t is 64‑bit, cast to __uint128_t to avoid overflow. */
    if (sizeof(seq_t) == 8) {
        __uint128_t tmp = (__uint128_t)x * (__uint128_t)num;
        return (seq_t)(tmp / den);
    }
#endif
    /* Generic case: rely on seq_t's own arithmetic (big‑int, 32‑bit, …). */
    return (x * num) / den;
}

/* Helper: floor(sqrt(x)) for x < 2^64 */
static inline idx_t floor_isqrt(idx_t x)
{
    idx_t low = 0, high = 0xFFFFFFFFULL;           /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* locate row r and column c from the flattened index n */
    idx_t s = floor_isqrt(8 * n + 1);          /* s = floor(sqrt(8n+1))      */
    idx_t r = (s - 1) >> 1;                     /* r = (s-1)/2                */
    idx_t tri = r * (r + 1) >> 1;               /* r*(r+1)/2                  */
    idx_t c = n - tri;                          /* column within row r        */

    /* compute T(r,c) by the vertical recurrence */
    seq_t val = 1;  /* T(m,m)=1 when i==c starts the loop */
    for (idx_t i = c + 1; i <= r; ++i) {
        seq_t num = 2 * ((seq_t)(2 * i - c - 1));  /* 2*(2*i - c - 1)   */
        seq_t den = i - c;                        /* i - c             */
        val = safe_mul_div(val, num, den);
    }
    return val;
}
