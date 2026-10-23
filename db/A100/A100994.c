/*
 * A100994  If n is a prime power p^m, m >= 1, then n, otherwise 1.
 * https://oeis.org/A100994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is a prime power p^m (m ≥ 1), 1 otherwise
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_power_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_power_idx.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;            /* a(1) = 1 */
    return (is_prime_power_idx(n)) ? (seq_t)n : (seq_t)1;
}
