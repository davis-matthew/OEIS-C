/*
 * A031405  Numbers k such that the continued fraction for sqrt(k) has even
 *          period 2*m and the m-th term of the periodic part is 2.
 * https://oeis.org/A031405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A031405: numbers k such that the continued fraction
   for sqrt(k) has an even period 2*m and the m-th term of the periodic
   part equals 2.  The function returns the n‑th such integer for n >= 1. */
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root of a 64‑bit unsigned integer, floor value.  */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2U) return n;
    uint64_t low  = 0;
    uint64_t high = 1;
    /* find an upper bound by doubling until high^2 > n */
    while (high <= n / high) ++high;
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (mid <= n / mid) low = mid;
        else                high = mid;
    }
    return low;
}

/* Test whether D satisfies the property.  D is assumed > 1 and not a
   perfect square.  */
static bool qualifies(uint64_t D)
{
    const uint64_t a0 = isqrt_u64(D);
    if (a0 * a0 == D) return false;   /* perfect square */

    uint64_t m   = 0;
    uint64_t d   = 1;
    uint64_t a1  = a0;   /* previous a */
    uint64_t a   = a0;
    uint64_t period = 0;

    /* Generate the continued–fraction terms until a == 2*a0, which
       indicates that the period has closed. */
    while (true) {
        m = d * a1 - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        ++period;
        if (a == 2 * a0) break;
        a1 = a;
    }

    if (period & 1ULL) return false;   /* odd period */

    /* The m-th term (mid point of the period) must be 2. */
    const uint64_t mid = period / 2;

    /* Recompute up to the mid‑term to inspect its value. */
    m   = 0;
    d   = 1;
    a1  = a0;
    for (uint64_t i = 1; i <= mid; ++i) {
        m = d * a1 - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        if (i == mid && a == 2) return true;
        a1 = a;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n == 0 is undefined; return 0 for safety, but the caller
       guarantees n >= 1. */
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    uint64_t candidate = 2;   /* start from the smallest non‑square > 1 */

    while (true) {
        if (qualifies(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
