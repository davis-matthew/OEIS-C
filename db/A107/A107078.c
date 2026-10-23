/*
 * A107078  Whether n has non-unitary prime divisors.
 * https://oeis.org/A107078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n has a non‑unitary prime divisor (i.e. n is not
 *            square‑free), 0 otherwise. Equivalently a(n) = 1 - μ(n)^2
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   has_square_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_square_prime.h"

/* External sequence function required by the test harness. */
seq_t a(idx_t n)
{
    /* n is the index, which is also the argument of the mathematical
       definition.  We return 1 when n is not square‑free, otherwise 0. */
    return (seq_t)(has_square_prime(n) ? 1 : 0);
}
