/*
 * A204562  Symmetric matrix: f(i,j) = floor((2i+2j+6)/4)-floor((i+j+3)/4), by
 *          (constant) antidiagonals.
 * https://oeis.org/A204562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let s be the smallest integer with s(s-1)/2 ≥ n. Then a(n) =
 *            floor((s+1)/4) + 1. (Equivalently, a(n) =
 *            floor((ceil((1+sqrt(1+8n))/2)+1)/4)+1.)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for the offset 1; safe fallback */

    /* Find the minimal s such that s(s-1)/2 ≥ n */
    idx_t low = 1, high = 2;
    while ((__int128)high * (high - 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 val = (__int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    seq_t result = (seq_t)((s + 1) / 4) + 1;
    return result;
}
