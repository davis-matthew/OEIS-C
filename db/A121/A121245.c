/*
 * A121245  (Floor(n*Pi))^n.
 * https://oeis.org/A121245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (floor(n*Pi))^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, get_pi_scaled
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/get_pi_scaled.h"

seq_t a(idx_t n)
{
    unsigned __int128 pi, pow10;
    get_pi_scaled(&pi, &pow10);

    /* floor(n * Pi) = floor((n * pi) / 10^30) */
    unsigned __int128 prod = (unsigned __int128)n * pi;
    unsigned __int128 floor_val = prod / pow10;

    seq_t base = (seq_t)floor_val;
    seq_t exp  = (seq_t)n;
    return big_pow(base, exp);
}
