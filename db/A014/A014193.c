/*
 * A014193  n-th prime + mu(n).
 * https://oeis.org/A014193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + μ(n)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (where prime(n) is the nth prime, μ(n) is the Möbius function) */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)get_prime(n); /* nth prime (1‑based) */
    val += mobius(n);                /* add Möbius function value */
    return val;
}
