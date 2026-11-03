/*
 * A169879  Maximum number of rational points on a smooth absolutely
 *          irreducible projective curve of genus 3 over the field F_3^n.
 * https://oeis.org/A169879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3^n + 1 + 6 * floor(sqrt(3^n)); For n = 3 the actual
 *            maximum is 56, so a(3) = 56
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"

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
    /* compute q = 3^n */
    seq_t base = (seq_t)3;
    seq_t q = ipow(base, n);

    /* s = floor(sqrt(q)) */
    seq_t s = floor_sqrt(q);

    /* a = q + 1 + 6 * s */
    seq_t a_val = q + (seq_t)1;
    a_val += s * (seq_t)6;

    /* special adjustment for n = 3 */
    if (n == (idx_t)3)
        a_val -= (seq_t)2;

    return a_val;
}
