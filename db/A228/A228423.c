/*
 * A228423  Sum of the squared primes less than or equal to n.
 * https://oeis.org/A228423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all p² where p is a prime and p² ≤ n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Sum of the squared primes less than or equal to n (A228423). */

#include <stdbool.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    while (true)
    {
        idx_t y = x + 1;
        if (y > n / y) break;
        ++x;
    }
    return x;
}

static inline bool is_prime_simple(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i <= x / i; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t sum = 0;
    idx_t limit = isqrt_u64(n);

    for (idx_t p = 2; p <= limit; ++p)
    {
        if (is_prime_simple(p))
            sum += (seq_t)p * (seq_t)p;
    }

    return sum;
}
