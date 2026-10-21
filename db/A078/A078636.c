/*
 * A078636  a(n) = rad(n*(n+1)).
 * https://oeis.org/A078636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(n) * rad(n+1) where rad(k) = product of the distinct
 *            prime factors of k. The function computes the radical of each
 *            integer by trial division. It is safe for all n ≥ 1 and works
 *            for any seq_t / idx_t definitions provided by the build
 * Range:     u8 n <= 20, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rad_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/rad_u64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    uint64_t n_val = (uint64_t)n;
    seq_t rad_n   = rad_u64(n_val);
    seq_t rad_np1 = rad_u64(n_val + 1);
    return rad_n * rad_np1;
}
