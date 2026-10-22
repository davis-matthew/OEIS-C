/*
 * A005883  Theta series of square lattice with respect to deep hole.
 * https://oeis.org/A005883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * #{(i,j) : i,j >= 0, i(i+1)/2 + j(j+1)/2 = n}, i.e.
 *            G.f. 4*psi(x)^2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005883_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005883_isqrt.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t i = 0;

    while (1) {
        uint64_t ti = i * (i + 1) / 2;
        if (ti > (uint64_t)n) break;

        uint64_t remainder = (uint64_t)n - ti;
        /* remainder is triangular iff 8*remainder + 1 is an odd perfect square */
        uint64_t disc = 8 * remainder + 1;
        uint64_t s = a005883_isqrt(disc);
        if (s * s == disc) count++;

        i++;
    }

    return (seq_t)count * 4;
}
