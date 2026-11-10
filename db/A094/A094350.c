/*
 * A094350  Numbers k such that A094291(k) is not a square.
 * https://oeis.org/A094350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A094291(k) is not a square. Search:
 *            A094291(k) is the maximum of the chain f(j)=C(k-j,j)^2 and
 *            g(j)=C(k-j-1,j)*C(k-j,j+1), interleaved as
 *            f(0),g(0),f(1),g(1),... Consecutive ratios are small rational
 *            numbers, g(j)/f(j)=(k-2j)^2/((k-j)(j+1)) and
 *            f(j+1)/g(j)=(k-2j-1)^2/...
 * Range:     u8 n <= 102, u16 n <= 960, u32 n <= 960, u64 n <= 960,
 *            u128 n <= 960, big any n
 * Validated: standard, pass, 79 terms through n = 891 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A094350_is_sq(unsigned long long x)
{
    unsigned long long lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        unsigned long long mid = (lo + hi) / 2;
        if (mid * mid < x) lo = mid + 1; else hi = mid;
    }
    return lo * lo == x;
}

/* true if A094291(k) is not a square */
static bool A094350_nonsq(unsigned long long k)
{
    unsigned long long j = 0;
    for (;;) {
        /* step from f(j) to g(j) */
        unsigned long long a = k - 2 * j;
        unsigned long long num = a * a;
        unsigned long long den = (k - j) * (j + 1);
        if (num == den) return false;   /* tie with f(j): square */
        if (num < den) return false;    /* max is f(j): square */
        /* step from g(j) to f(j+1) */
        unsigned long long b = a - 1;
        num = b * b;
        if (num == den) return false;   /* tie with f(j+1): square */
        if (num < den) {
            /* max is g(j) */
            return !A094350_is_sq((k - j) * (j + 1));
        }
        ++j;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    unsigned long long k = 0;
    while (count < n) {
        ++k;
        if (A094350_nonsq(k)) ++count;
    }
    return (seq_t)k;
}
