/*
 * A051747  a(n) = n*(n+1)*(n+2)*(n^2+7*n+32)/120.
 * https://oeis.org/A051747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n^2+7*n+32)/120
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 217, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate product to avoid
       overflow on 8/16/32/64‑bit seq_t implementations.  The final
       division by 120 guarantees the result fits in seq_t whenever
       the value fits in the tested range. */
    unsigned __int128 x = (unsigned __int128)n;
    unsigned __int128 prod = x * (x + 1) * (x + 2) *
                             (x * x + 7 * x + 32);
    prod /= 120;
    return (seq_t)prod;
}
