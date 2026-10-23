/*
 * A190191  a(n) = [n*u+n*v]-[n*u]-[n*v], where u=sqrt(2), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n·(√2 + 1/√2)⌋ – ⌊n·√2⌋ – ⌊n/√2⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic, no floating point. */
#include <stdint.h>
#include "../utils/floor_sqrt2.h"

seq_t a(idx_t n) {
    /* compute ⌊n·√2⌋ */
    uint64_t nu = floor_sqrt2(n);

    /* compute ⌊n/√2⌋ = ⌊⌊n·√2⌋ / 2⌋, because division by 2 preserves the floor */
    uint64_t nv = nu / 2;

    /* compute ⌊n·(√2 + 1/√2)⌋ = ⌊(3n)·√2 / 2⌋ = ⌊⌊(3n)·√2⌋ / 2⌋ */
    uint64_t nu_plus_nu_1over_sqrt2 = floor_sqrt2(3 * (uint64_t)n) / 2;

    /* a(n) = ⌊(3n)·√2/2⌋ – ⌊n·√2⌋ – ⌊n/√2⌋  (always 0 or 1) */
    uint64_t val = nu_plus_nu_1over_sqrt2 - nu - nv;

    return (seq_t)val;
}
