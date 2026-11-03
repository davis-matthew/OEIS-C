/*
 * A213673  (n^2 - A000695(n))/4.
 * https://oeis.org/A213673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*n - A000695(n)) / 4 where A000695(n) (pseudo‑square
 *            function) maps each bit k of n to bit 2k (i.e., inserts a zero
 *            bit between consecutive bits). PSF(n) can be computed by
 *            iterating over the set bits of n and accumulating 1ULL << (2*k)
 * Range:     u8 n <= 23, u16 n <= 425, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 75 terms through n = 996 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pseudo_square(idx_t n)
/* Compute PSF(n) = ∑_{k} ((n >> k) & 1) << (2*k) */
{
    seq_t result = 0;
    seq_t shift   = 1;               /* 1 << (2*0) */
    while (n != 0) {
        if (n & 1) {
            result += shift;
        }
        shift <<= 2;                 /* move to bit 2*(k+1) */
        n >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
/* a(n) = (n^2 - PSF(n)) / 4 */
{
    seq_t psf   = pseudo_square(n);
    seq_t sq    = (seq_t)n * (seq_t)n;
    seq_t diff  = sq - psf;          /* always divisible by 4 */
    return diff >> 2;               /* division by 4 */
}
