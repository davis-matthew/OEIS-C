/*
 * A054414  a(n) = 1 + floor(n/(1-log(2)/log(3))).
 * https://oeis.org/A054414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + floor( n * log(3) / log(3/2) ) (≈ 1 +
 *            floor(2.709511367220874... * n))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned long long SCALE = 1000000000000ULL;     /* 10^12 */
static const unsigned long long NUM   = 2709511367220ULL;     /* floor(r * SCALE) */

/* Computes A054414(a(n) = 1 + floor(n * log(3) / log(3/2))) */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow:  n * NUM  < 2^80 for n < 10^8 */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    /* Floor division of the scaled product gives floor(n * r) */
    seq_t val = (seq_t)(prod / SCALE + 1);
    return val;
}
