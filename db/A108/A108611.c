/*
 * A108611  Excess of Beatty-function of 1/sin(1) over n.
 * https://oeis.org/A108611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*(1/sin(1)-1))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here α = 1/sin(1)-1 ≈ 0.188395058279.  We avoid floating‑point by using
   a scaled integer representation with 12 fractional digits. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Scaling factor 10^12 to preserve 12 decimal places */
    const idx_t SCALE      = 1000000000000ULL;   /* 1e12          */
    /* α scaled: (1/sin(1)-1) * 10^12 ≈ 188395058279 */
    const idx_t NUM_ALPHA  = 188395058279ULL;

    /* floor((n+1) * α) = floor((n+1) * NUM_ALPHA / SCALE) */
    __uint128_t prod  = (__uint128_t)(n + 1) * NUM_ALPHA;
    seq_t result      = (seq_t)(prod / SCALE);
    return result;
}
