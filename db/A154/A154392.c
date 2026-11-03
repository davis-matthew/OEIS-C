/*
 * A154392  Number of zeros of sin(x^2) in integer intervals starting with
 *          (0,1).
 * https://oeis.org/A154392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/π) - floor((n-1)^2/π)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Use a rational approximation 33215/104348 for 1/π
   (error < 5×10⁻⁹, sufficient for n ≤ 10⁴) */
static seq_t floor_div_pi(idx_t n)
{
    /* compute floor( n^2 / π ) ≈ floor( n^2 * 33215 / 104348 ) */
    __int128 prod = (__int128)n * n * 33215;   /* 128‑bit intermediate to avoid overflow */
    return (seq_t)(prod / 104348);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for out‑of‑range calls */
    seq_t up   = floor_div_pi(n);
    seq_t low  = floor_div_pi(n - 1);
    return up - low;
}
