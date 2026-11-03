/*
 * A140208  a(n) = floor(n*pi(n)/2) where pi(n) is the number of primes <= n.
 * https://oeis.org/A140208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * pi(n) / 2)
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* pi(n) gives the number of primes ≤ n */
    seq_t pi = prime_pi(n);

    /* Calculate n * pi(n) and then divide by 2 */
    seq_t prod = ((seq_t)n) * pi;
    return prod / 2;
}
