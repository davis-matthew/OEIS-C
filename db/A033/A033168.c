/*
 * A033168  Longest arithmetic progression of primes with difference 210 and
 *          minimal initial term.
 * https://oeis.org/A033168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = start + n*210 for 0 <= n < 10, where start is the minimal
 *            prime p such that p, p+210, …, p+210*9 are all prime
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* TODO: make sure a(idx_t) returns the correct terms for the
   finite sequence A033168: 199, 409, 619, 829, 1039, 1249,
   1459, 1669, 1879, 2089. */

#include <stdbool.h>
#include <stdint.h>

/* Simple primality test for 64-bit integers (sufficient for this
   sequence: values are < 3,000). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Find the minimal starting prime for a 10‑term AP with difference 210. */
static seq_t find_start(void)
{
    for (uint64_t p = 2; ; ++p)
    {
        if (!is_prime_uint64(p)) continue;
        bool ok = true;
        for (int i = 1; i < 10; ++i)
        {
            if (!is_prime_uint64(p + 210ULL * (uint64_t)i))
            {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)p;
    }
}

/* Compute a(n) for the sequence. */
seq_t a(idx_t n)
{
    /* static cache: store the start once the first time it is needed */
    static seq_t start = 0;
    static bool initialized = false;

    if (!initialized)
    {
        start = find_start();
        initialized = true;
    }

    if (n < 10)
    {
        /* (seq_t)n ensures the multiplication occurs in seq_t */
        return start + (seq_t)n * (seq_t)210;
    }
    else
    {
        /* For indices beyond the defined length, return 0.
           The sequence is finite, so this is acceptable. */
        return (seq_t)0;
    }
}
