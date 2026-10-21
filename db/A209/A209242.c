/*
 * A209242  The largest fixed value (neither happy nor sad) in base n.
 * https://oeis.org/A209242
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest fixed point in base n:
 *  a(n) = max{ a + b*n | 0 <= a < n, 1 <= b < n, a + b*n = a^2 + b^2 }
 *  (the base-10 value of the number whose base-n representation is digits
 *   (b,a)). This is equivalent to searching all possible two‑digit numbers
 *   in base n for solutions of a^2 + b^2 = a + b*n. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Base 1 is always a fixed point. For n<3 the offset is
     * undefined; just return 1. */
    if (n < 3) return (seq_t)1;

    seq_t base = (seq_t)n;
    seq_t max_val = (seq_t)1;            /* 1 is always a fixed point */

    for (idx_t b = 1; b < n; ++b) {
        seq_t bval = (seq_t)b;
        seq_t t = (seq_t)1 + (seq_t)4 * bval * (base - bval);  /* 1 + 4b(n-b) */
        seq_t s = floor_sqrt(t);
        if (s * s != t) continue;                 /* discriminant not perfect square */

        seq_t aval = (s + (seq_t)1) >> 1;          /* a = (1 + s)/2 */
        if (aval >= base) continue;               /* digit out of range */

        seq_t val = aval + bval * base;           /* the fixed point value */
        if (val > max_val) max_val = val;
    }

    return max_val;
}
