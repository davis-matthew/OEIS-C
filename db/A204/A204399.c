/*
 * A204399  Numbers k such that floor(2^k / 3^n) = 1.
 * https://oeis.org/A204399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n * log_2(3)) (OEIS A204399)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  f = log2(3) - 1 = log2(3/2)  ≈ 0.5849625007211562
 *  We use fixed‑point with a 32‑bit fractional part:
 *      f ≈ F / 2^32   where F = floor(f * 2^32) = 2512394809.
 */
static const unsigned __int128 F_VALUE = (unsigned __int128)2512394809ULL;
static const unsigned __int128 SCALE   = (unsigned __int128)1ULL << 32U;

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return 0;

    /* floor(n * f) = floor( (n * F_VALUE) / SCALE ) */
    unsigned __int128 prod = (unsigned __int128)n * F_VALUE;
    idx_t floor_nf = (idx_t)(prod >> 32U); /* division by 2^32 */

    /* a(n) = n + 1 + floor(n * f) */
    return (seq_t)(n + 1 + floor_nf);
}
