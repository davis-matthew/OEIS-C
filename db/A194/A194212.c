/*
 * A194212  a(n) = floor(Sum_{k=1..n} <2*k*Pi>), where < > = fractional part.
 * https://oeis.org/A194212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Sum_{k=1..n} fract( 2*k*Pi ) )
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here we approximate Pi by 3141592653590 / 10^12 (error < 5×10^-13)
   and accumulate the scaled fractional parts.  The sum is stored as
   an integer multiple of the scaling factor; integer division yields the
   desired floor of the real sum.
   The method is exact for the sequence for n up to 10,000, as the total
   approximation error is < 10^-4 << 0.5.  It is O(n) and uses only
   integer arithmetic. */
#include <stdint.h>

static const unsigned long long SCALE = 1000000000000ULL;   /* 10^12 */
static const unsigned long long PI_SCALE = 3141592653590ULL; /* Pi * 10^12 (rounded) */

seq_t a(idx_t n)
{
    unsigned __int128 prod;       /* product 2*PI_SCALE*k may fit in 64‑bit, but use 128‑bit for safety */
    unsigned long long sum = 0;   /* sum of scaled fractional parts, < 1e16 for n <= 1e4  */
    idx_t k;

    for (k = 1; k <= n; ++k)
    {
        prod = (unsigned __int128)2 * PI_SCALE * k;   /* 2 * Pi * k, scaled */
        sum += (unsigned long long)(prod % SCALE);    /* add fractional part * S */
    }

    return (seq_t)(sum / SCALE);  /* floor of the real sum */
}
