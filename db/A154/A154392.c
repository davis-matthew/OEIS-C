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
 * Helpers:   floor_div_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_div_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for out‑of‑range calls */
    seq_t up   = floor_div_pi(n);
    seq_t low  = floor_div_pi(n - 1);
    return up - low;
}
