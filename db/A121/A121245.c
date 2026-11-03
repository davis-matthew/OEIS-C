/*
 * A121245  (Floor(n*Pi))^n.
 * https://oeis.org/A121245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (floor(n*Pi))^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 197 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/*  Compute the scaled value of Pi and 10^30.
    Pi_scaled = floor(Pi * 10^30)  (integer part + 30 decimal places)
    pow10     = 10^30               */
static inline void get_pi_scaled(unsigned __int128 *pi, unsigned __int128 *pow10)
{
    /* Pi = 3.141592653589793238462643383279... */
    const char *digits = "141592653589793238462643383279";
    unsigned __int128 p = 3;
    for (int i = 0; digits[i]; ++i)
        p = p * 10 + (unsigned)(digits[i] - '0');
    *pi = p;

    /* pow10 = 10^30 */
    unsigned __int128 pow = 1;
    for (int i = 0; i < 30; ++i)
        pow *= 10;
    *pow10 = pow;
}

seq_t a(idx_t n)
{
    unsigned __int128 pi, pow10;
    get_pi_scaled(&pi, &pow10);

    /* floor(n * Pi) = floor((n * pi) / 10^30) */
    unsigned __int128 prod = (unsigned __int128)n * pi;
    unsigned __int128 floor_val = prod / pow10;

    seq_t base = (seq_t)floor_val;
    seq_t exp  = (seq_t)n;
    return ipow(base, exp);
}
