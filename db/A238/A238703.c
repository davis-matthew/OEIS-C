/*
 * A238703  a(n) = |{0 < k < n: floor(k*n/3) is prime}|.
 * https://oeis.org/A238703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : floor(k*n/3) is prime}|
 * Range:     u8 n <= 3639, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* small prime test for numbers up to a few thousand, works on any seq_t size */
static inline bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v == 2 || v == 3) return true;
    if ((v & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= v; d += 2)
    {
        if (v % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        uint64_t t = (uint64_t)k * (uint64_t)n / 3;   /* floor(k*n/3) */
        if (is_prime_uint64(t))
            ++count;
    }
    return count;
}
