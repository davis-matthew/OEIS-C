/*
 * A067628  Minimal perimeter of polyiamond with n triangles.
 * https://oeis.org/A067628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal perimeter of a polyiamond with n triangles. The
 *            sequence satisfies a(n) = whichever of ceil(sqrt(6n)) or
 *            ceil(sqrt(6n)) + 1 has the same parity as n. We compute
 *            ceil(sqrt(6n)) using integer arithmetic
 * Range:     u8 n <= 10, u16 n <= 1948, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute floor(sqrt(x)) for non‑negative x in seq_t. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;                /* sqrt(x) ≤ x for x≥0 */
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x) {
            ans = mid;             /* mid is a candidate */
            low = mid + 1;         /* try a larger value */
        } else {
            high = mid - 1;        /* try a smaller value */
        }
    }
    return ans;
}

/* Compute ceil(sqrt(x)). */
static seq_t ceil_sqrt(seq_t x)
{
    seq_t f = floor_sqrt(x);
    if (f * f == x)
        return f;
    return f + 1;
}

seq_t a(idx_t n)
{
    /* Compute ceil(sqrt(6*n)) */
    seq_t six_n = (seq_t)n * 6;
    seq_t c = ceil_sqrt(six_n);

    /* Choose c or c+1 according to the parity of n. */
    if ( (c & 1) == ((seq_t)n & 1) )
        return c;
    else
        return c + 1;
}
