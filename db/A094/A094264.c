/*
 * A094264  a(n) = Sum_{r = 1..n} (n^r mod r).
 * https://oeis.org/A094264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r = 1..n} (n^r mod r)
 * Range:     u8 n <= 37, u16 n <= 529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t r = 1; r <= n; ++r)
    {
        /* Compute n^r mod r using the provided modular exponentiation helper */
        seq_t term = mod_pow((seq_t)n, (uint64_t)r, (seq_t)r);
        total += term;
    }
    return total;
}
