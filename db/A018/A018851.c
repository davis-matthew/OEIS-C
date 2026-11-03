/*
 * A018851  a(n)^2 is smallest square beginning with n.
 * https://oeis.org/A018851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 9457, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 89 terms through n = 9457 (2026-11-01)
 * Helpers:   ipow_i32, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) where a(n)^2 is the smallest square that begins with the
 * decimal representation of n.  The algorithm checks increasing counts
 * of trailing digits d until a square lies in the interval
 * [n·10^d, (n+1)·10^d – 1].  The square root of that square is the return value. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits_idx.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 (the supplied sequence definition
       agrees with a(0) = 0). */
    if (n == 0)
        return (seq_t)0;

    seq_t base = (seq_t)n;    /* the prefix to be matched */

    for (idx_t d = 0; ; ++d) {
        seq_t pow10d = pow10(d);              /* 10^d  */
        seq_t low  = base * pow10d;           /* n·10^d      */
        seq_t high = (base + 1) * pow10d - 1; /* (n+1)·10^d – 1 */

        /* Find the smallest m with m^2 >= low. */
        seq_t m = floor_sqrt(low);
        if (m*m < low) ++m;

        /* Check if this m satisfies m^2 <= high. */
        seq_t m2 = m * m;
        if (m2 <= high)
            return m;  /* a(n) found */

        /* Otherwise increase the number of trailing digits d. */
    }
}
