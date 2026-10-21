/*
 * A095703  Integer part of the (positive) solution to x^x = 10^n.
 * https://oeis.org/A095703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the integer part of the (positive) solution of x^x =
 *            10^n. Since y·log10(y) is strictly increasing for y ≥ 1, a(n) =
 *            max{y∈ℕ : y·log10(y) ≤ n}
 * Range:     u8 n <= 616, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* 1^1 = 1 = 10^0 */
        return (seq_t)1;

    /* Upper bound: solution is < n, n+10 is comfortably larger. */
    uint64_t lo = 1;
    uint64_t hi = n + 10;   /* hi guaranteed to satisfy hi·log10(hi) > n */

    long double target = (long double)n;

    /* Binary search for first y with y·log10(y) > n. */
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        long double val = (long double)mid * log10l((long double)mid);
        if (val <= target)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first y with y·log10(y) > n, so answer is lo‑1. */
    return (seq_t)(lo - 1);
}
