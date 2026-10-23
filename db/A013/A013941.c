/*
 * A013941  a(n) = Sum_{k = 1..n} floor(n/prime(k)^3).
 * https://oeis.org/A013941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 1..n} floor(n / prime(k)^3)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int is_prime_small(uint64_t x)
{
    if (x < 2) return 0;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; defensive guard */

    seq_t result = 0;
    for (uint64_t p = 2; ; ++p) {
        uint64_t p3 = (uint64_t)p * p * p;
        if (p3 > n) break;
        if (!is_prime_small(p)) continue;
        /* n and p3 are idx_t; cast to seq_t after division */
        result += (seq_t)(n / p3);
    }
    return result;
}
