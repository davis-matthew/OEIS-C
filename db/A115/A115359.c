/*
 * A115359  Matrix (1,x)-(x,x^2) in Riordan array notation.
 * https://oeis.org/A115359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = A010054(n) - A379480(n) Equivalent: a(n) = 1 if n =
 *            k(k+3)/2 (k ≥ 0) a(n) = -1 if n = 2k^2 - 1 (k ≥ 1) otherwise 0
 * Range:     i8 n <= 22154, i16 n <= 22154, i32 n <= 22154, i64 n <= 22154,
 *            i128 n <= 22154, big any n
 * Validated: standard, pass, 136 terms through n = 9824 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for 128‑bit values.
   If x is a perfect square, returns true and sets *root to √x.
   Otherwise returns false. */
static bool is_perfect_square128(__uint128_t x, uint64_t *root)
{
    __uint128_t lo = 0, hi = x;
    while (lo <= hi) {
        __uint128_t mid = (lo + hi) >> 1;
        if (mid == 0) {  /* avoid division by zero */
            lo = 1;
            continue;
        }
        if (mid <= x / mid)      /* mid^2 ≤ x  */
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    uint64_t r = (uint64_t)hi;               /* hi is ⌊√x⌋ */
    if ((__uint128_t)r * r == x) {
        *root = r;
        return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Check the +1 case: n = k(k+3)/2  →  k^2 + 3k - 2n = 0
       ⇒ discriminant D = 9 + 8n must be a perfect square. */
    __uint128_t D = (__uint128_t)9 + (__uint128_t)8 * n;
    uint64_t rootD;
    if (is_perfect_square128(D, &rootD)) {
        if (rootD >= 3 && ((rootD - 3) & 1ULL) == 0) {
            /* k = (rootD - 3)/2 ≥ 0 */
            return (seq_t)1;
        }
    }

    /* Check the -1 case: n = 2k^2 - 1  →  k^2 = (n+1)/2
       Only possible if n is odd. */
    if ((n & 1ULL) == 1) {                     /* n is odd → n+1 even */
        __uint128_t m = (__uint128_t)n + 1;     /* (n+1)/2 in 128‑bit */
        m >>= 1;
        uint64_t rootM;
        if (is_perfect_square128(m, &rootM)) {
            if (rootM >= 1)                     /* k≥1 */
                return (seq_t)-1;
        }
    }

    return (seq_t)0;
}
