/*
 * A053191  a(n) = n^2 * phi(n).
 * https://oeis.org/A053191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^2 * phi(n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9987 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

seq_t a(idx_t n)
{
    /* n >= 1 per OEIS offset */
    uint64_t n64 = (uint64_t)n;
    uint64_t phi = totient_u64(n64);
    unsigned __int128 prod = (unsigned __int128)n64 * n64 * phi;
    return (seq_t)prod;
}
