/*
 * A123123  Numbers m such that m mod k = 2 for only one integer k in 2..m.
 * https://oeis.org/A123123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 2 for n ≥ 3; a(1) = 5, a(2) = 6
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)5;
    if (n == (idx_t)2) return (seq_t)6;
    /* For n ≥ 3, a(n) equals the n‑th prime plus 2. */
    return (seq_t)get_prime(n) + (seq_t)2;
}
