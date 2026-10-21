/*
 * A065730  Largest square <= n-th prime.
 * https://oeis.org/A065730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest square ≤ the n‑th prime (OEIS A065730)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; undefined for 0 */
    seq_t p = (seq_t)get_prime(n); /* n‑th prime, 1‑based index */
    seq_t s = floor_sqrt(p);       /* floor(sqrt(p)) */
    return s * s;                  /* largest square ≤ p */
}
