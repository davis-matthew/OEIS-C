/*
 * A108139  n to the power of the largest prime divisor of n.
 * https://oeis.org/A108139
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(largest prime divisor of n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   largest_prime_factor, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/largest_prime_factor.h"

/* OEIS A108139: n^(largest prime divisor of n) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2, return 0 */
    idx_t lp   = largest_prime_factor(n);
    return pow_seq((seq_t)n, lp);
}
