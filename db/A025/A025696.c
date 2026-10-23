/*
 * A025696  Index of 3^n within the sequence of the numbers of the form
 *          3^i*4^j.
 * https://oeis.org/A025696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} ceil(k * log_4(3))
 * Range:     u8 n <= 24, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here log_4(3) ≈ 0.792481250969…  */
/* The product k*log_4(3) is computed with an integer approximation
   using 12‑digit fixed point: NUM / SCALE = 0.792481250969.  */

#include <stdint.h>

static const uint64_t SCALE = 1000000000000ULL;   /* 10^12 */
static const uint64_t NUM   = 792481250969ULL;     /* round(log_4(3) * SCALE) */

seq_t a(idx_t n)
{
    seq_t ans = 1;          /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute ceil(k * NUM / SCALE) = floor(k*NUM/SCALE) + 1   */
        uint64_t prod  = k * NUM;          /* fits in 64‑bit (k≤10^4)   */
        seq_t   inc   = (seq_t)(prod / SCALE) + 1;
        ans += inc;
    }
    return ans;
}
