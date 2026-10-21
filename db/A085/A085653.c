/*
 * A085653  a(n) = smallest integer > n where mu(n) = mu(a(n)).
 * https://oeis.org/A085653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer > n with μ(n) = μ(a(n))
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_u64.h"

/*
 * Smallest integer > n having the same Möbius value as n.
 */
seq_t a(idx_t n) {
    int mu_n = mobius_u64(n);
    idx_t m = n + 1;
    while (1) {
        if (mobius_u64(m) == mu_n) return (seq_t)m;
        ++m;
    }
}
