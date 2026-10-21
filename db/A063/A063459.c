/*
 * A063459  A Beatty sequence: a(n) = floor(n*(Pi - 1)).
 * https://oeis.org/A063459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(Pi - 1)) = floor(n*Pi) - n
 * Range:     u8 n <= 119, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A fixed-point representation of Pi scaled by 10^18.  */
/* Pi ≈ 3.14159265358979323846264338327950288...  */
static const unsigned long long PI_FIXED = 3141592653589793238ULL;
/* Scale factor 10^18 */
static const unsigned long long SCALE   = 1000000000000000000ULL;

seq_t a(idx_t n)
{
    /* Compute floor(n * Pi) using fixed‑point arithmetic.
       The product n * PI_FIXED easily overflows 64‑bit, so use
       unsigned __int128 for intermediate precision. */
    unsigned __int128 prod = (unsigned __int128)(unsigned long long)n * PI_FIXED;
    unsigned __int128 div  = prod / SCALE;          /* floor(n*Pi)      */
    seq_t val = (seq_t)div;                         /* cast to seq_t   */
    return val - (seq_t)n;                          /* floor(n*(Pi-1)) */
}
