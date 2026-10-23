/*
 * A015910  a(n) = 2^n mod n.
 * https://oeis.org/A015910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n mod n
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at n = 1 */
    return mod_pow((seq_t)2, (uint64_t)n, (seq_t)n);
}
