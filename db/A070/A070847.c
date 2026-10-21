/*
 * A070847  Smallest prime == 1 mod (3n).
 * https://oeis.org/A070847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 950, u32 n <= 1982, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest prime congruent to 1 (mod 3*n) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; for n < 1 return 0 (undefined but harmless) */
    if (n == 0) return 0;

    /* modulus for this n */
    idx_t m = 3 * n;          /* 3*n fits in idx_t for all relevant n */

    /* first candidate in the arithmetic progression: 1 + k·(3n) with k=1 */
    seq_t candidate = (seq_t)1 + (seq_t)m;
    seq_t step      = (seq_t)m;

    /* Find the first prime in the progression */
    while (!is_prime_big(candidate))
        candidate += step;

    return candidate;
}
