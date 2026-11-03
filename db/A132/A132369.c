/*
 * A132369  a(n) = PrimePi(n)!.
 * https://oeis.org/A132369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime_pi(n)!
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 for the sequence; if called with n < 1 return 0 */
    if (n < 1)
        return 0;
    /* Count primes ≤ n and compute factorial of that count */
    idx_t pi = (idx_t)prime_pi(n);
    return factorial(pi);
}
