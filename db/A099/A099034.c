/*
 * A099034  a(n) = Sum_{k=1..n} (-1)^A000120(5*k).
 * https://oeis.org/A099034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} (-1)^A000120(5*k) (A000120 = binary
 *            popcount)
 * Range:     u8 n <= 5039, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 91 terms through n = 8643 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A099034: a(n) = Sum_{k=1..n} (-1)^popcount(5k), summed directly in 64 bits
 * (the original's cache read an unset entry when it grew). */

#include <stdint.h>

seq_t a(idx_t n)
{
    int64_t s = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; ++k)
        s += __builtin_parityll(5 * k) ? -1 : 1;
    return (seq_t)s;
}
