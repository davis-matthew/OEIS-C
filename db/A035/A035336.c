/*
 * A035336  a(n) = 2*floor(n*phi) + n - 1, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A035336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n*phi) + n - 1, phi = (1+sqrt(5))/2
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_n_sqrt5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_n_sqrt5.h"

seq_t a(idx_t n)
{
    /* floor(n*phi) = floor( (n + floor(n*sqrt(5)) ) / 2 ) */
    seq_t fn_sqrt5 = floor_n_sqrt5(n);
    seq_t floor_n_phi = ((seq_t)n + fn_sqrt5) / 2;

    /* a(n) = 2*floor(n*phi) + n - 1 */
    return 2 * floor_n_phi + (seq_t)n - 1ULL;
}
