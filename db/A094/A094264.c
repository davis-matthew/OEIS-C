/*
 * A094264  a(n) = Sum_{r = 1..n} (n^r mod r).
 * https://oeis.org/A094264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r = 1..n} (n^r mod r)
 * Range:     u8 n <= 37, u16 n <= 529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9947 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t r = 1; r <= n; ++r)
    {
        /* Compute n^r mod r using the provided modular exponentiation helper */
        seq_t term = powmod((seq_t)n, (uint64_t)r, (seq_t)r);
        total += term;
    }
    return total;
}
