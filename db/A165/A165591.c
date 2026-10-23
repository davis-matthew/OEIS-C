/*
 * A165591  Jacobi symbol (n,59701).
 * https://oeis.org/A165591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Jacobi symbol (n,59701)
 * Range:     i8 n <= 9496, i16 n <= 9496, i32 n <= 9496, i64 n <= 9496,
 *            i128 n <= 9496, big any n
 * Helpers:   powmod64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/powmod64_local.h"

/* Legendre symbol (a|p) for odd prime p.  Returns 1, 0, or -1. */
static int legendre(uint64_t a, uint64_t p)
{
    if (a % p == 0ULL) return 0;
    uint64_t r = powmod64_local(a % p, (p - 1ULL) >> 1, p);
    if (r == 1ULL) return 1;
    if (r == p - 1ULL) return -1;
    return 0;   /* unreachable for odd prime */
}

/* Jacobi symbol (n,59701) = (n|227)*(n|263) */
seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    const uint64_t p1 = 227ULL;
    const uint64_t p2 = 263ULL;
    int s1 = legendre(m, p1);
    if (s1 == 0) return (seq_t)0;
    int s2 = legendre(m, p2);
    if (s2 == 0) return (seq_t)0;
    return (seq_t)(s1 * s2);
}
