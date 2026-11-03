/*
 * A067479  Smallest n-digit square starting with 9, or 0 if no such number
 *          exists.
 * https://oeis.org/A067479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 0 for n=2; for odd n a(n) = 9·10^(n-1); for even n>2 a(n)
 *            = (⌊√(9·10^(n-1))⌋ + 1)^2 (the smallest n‑digit square starting
 *            with 9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    /* The sequence is defined for n>=1, with a(2)=0. */
    if (n == 2) return 0;

    /* If n is odd, the smallest n­digit square starting with 9 is 3·10^((n-1)/2) squared. */
    if ((n & 1) != 0) {
        seq_t p = pow10(n - 1);   /* 10^(n-1) */
        return 9 * p;             /* 9·10^(n-1) */
    }

    /* Even n>2: use the recurrence derived from the definition. */
    seq_t m = 9 * pow10(n - 1);    /* 9·10^(n-1) */
    seq_t s = floor_sqrt(m);      /* floor of the square root */
    s += 1;                       /* add (n-1) mod 2 which equals 1 for even n */
    return s * s;                 /* square of the adjusted root */
}
