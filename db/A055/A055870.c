/*
 * A055870  Signed Fibonomial triangle.
 * https://oeis.org/A055870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = (-1)^⌊(m+1)/2⌋ * fibonomial(r, m) where r = ⌊(√(8n+1) –
 *            1)/2⌋, m = n – r(r+1)/2, and fibonomial(r, m) = Π_{i=1..m}
 *            Fib(r+1-i) / Fib(i)
 * Range:     i8 n <= 30, i16 n <= 58, i32 n <= 110, i64 n <= 217,
 *            i128 n <= 416, big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/gcd.h"

static inline idx_t triangular(idx_t r)
{
    return (idx_t)((__int128)r * (r + 1) / 2);
}

static uint64_t isqrt_u128(__int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 35;   /* safe upper bound for any 128‑bit x */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t fibonomial(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t top = fib(n + 1 - i);  /* numerator term Fib(n+1-i) */
        seq_t bottom = fib(i);       /* denominator term Fib(i) */

        seq_t g = gcd(top, bottom);
        if (g > (seq_t)1) {
            top /= g;
            bottom /= g;
        }

        /* cancel common factor between current result and denominator */
        seq_t g2 = gcd(res, bottom);
        if (g2 > (seq_t)1) {
            res /= g2;
            bottom /= g2;
        }

        res = res * top;
        /* exact division – the product is an integer */
        res = res / bottom;
    }
    return res;
}

/* a(n) – signed Fibonomial triangle entry in row‑major order */
seq_t a(idx_t n)
{
    /* find the row r and index m within the row */
    __int128 v = (__int128)8 * n + 1;
    uint64_t s = isqrt_u128(v);
    uint64_t r = (s - 1ULL) >> 1;
    idx_t row = (idx_t)r;
    idx_t offset = triangular(row);
    idx_t m = n - offset;

    /* sign factor (-1)^floor((m+1)/2) */
    uint64_t exp = (m + 1ULL) >> 1;
    seq_t sign = ((exp & 1ULL) != 0) ? (seq_t)(-1) : (seq_t)1;

    seq_t fn = fibonomial(row, m);
    return sign * fn;
}
