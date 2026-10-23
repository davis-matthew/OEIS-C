/*
 * A013637  n*prevprime(n).
 * https://oeis.org/A013637
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * prev_prime(n)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;           /* behavior below offset is unused */
    return (seq_t)n * (seq_t)prev_prime(n);
}
