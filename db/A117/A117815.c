/*
 * A117815  Composite numbers at least one of whose decimal digits is prime.
 * https://oeis.org/A117815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the n-th composite number that has at least one decimal prime digit */

static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1ULL) == 0 || (x % 3ULL) == 0) return false;
    uint64_t i = 5;
    uint64_t step = 2;
    while (i <= x / i)
    {
        if (x % i == 0) return false;
        i += step;
        step = 6 - step;
    }
    return true;
}

static inline bool has_prime_digit_uint64(uint64_t x)
{
    while (x > 0)
    {
        uint8_t d = x % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, graceful fallback */

    idx_t target = n;
    idx_t count = 0;
    uint64_t num = 2;

    for (;; ++num)
    {
        if (!is_prime_uint64(num) && has_prime_digit_uint64(num))
        {
            ++count;
            if (count == target)
                return (seq_t)num;
        }
    }
}
