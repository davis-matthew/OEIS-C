/*
 * A033271  Number of odd nonprimes <= n.
 * https://oeis.org/A033271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/2) - primepi(n) + 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t odd_count = ((seq_t)n + 1) / 2;
    seq_t pi = prime_pi(n);
    return odd_count - pi + (seq_t)1;
}
