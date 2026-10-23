/*
 * A108582  n appears n^3 times.
 * https://oeis.org/A108582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 8281, u32 n <= 8281, u64 n <= 8281,
 *            u128 n <= 8281, big any n
 * Helpers:   cube_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = the smallest integer m ≥ 1 such that ((m*(m+1))/2)^2 ≥ n
 *   (i.e. the inverse of the sum of the first m cubes).
 *
 *  The implementation uses an integer binary search on m.  All
 *  intermediate arithmetic is performed in the unsigned integer type
 *  seq_t, guaranteeing that the result is exact for any value that
 *  fits in that type.
 */

#include <stdint.h> /* for uintptr_t; not strictly required but harmless */
#include "../utils/cube_sum.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* offset 1; undefined for n < 1, return 0 as safe default  */
        return 0;

    /* Find an upper bound hi such that cube_sum(hi) ≥ n. */
    idx_t lo = 0, hi = 1;
    seq_t S_hi = cube_sum(hi);
    while (S_hi < (seq_t)n) {
        lo = hi;
        hi <<= 1;                /* hi = 2*hi, safe because hi stays far below 2^63 */
        S_hi = cube_sum(hi);
    }

    /* Binary search for the minimal m with cube_sum(m) ≥ n. */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cube_sum(mid) >= (seq_t)n)
            hi = mid;
        else
            lo = mid;
    }

    return (seq_t)hi;  /* hi is the desired a(n) */
}
