/*
 * A035532  a(n) = 2*phi(n) if n composite, or 2*phi(n) - (A000120(n)-1) if n
 *          prime, where phi = A000010, Euler's totient function, and a(1) =
 *          1.
 * https://oeis.org/A035532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*phi(n) if n composite, otherwise * 2*phi(n) -
 *            (A000120(n)-1) for prime n, with a(1)=1. * (A000120 counts
 *            1‑bits of n, i.e. popcount(n).)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t phi_n = phi(n);
    seq_t result = (seq_t)2 * phi_n;

    if (is_prime(n)) {
        /* A000120(n) — number of set bits in n */
        unsigned int pop = __builtin_popcountll((unsigned long long)n);
        result -= (seq_t)(pop - 1);
    }

    return result;
}
