/*
 * A088429  Number of primes in arithmetic progression starting with 31 and
 *          with d=2n.
 * https://oeis.org/A088429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A088429: Number of primes in the arithmetic progression beginning at 31
   with common difference d = 2*n, terminated when the first non‑prime appears.
   a(n) = |{k ≥ 0 : 31 + 2*n*k is prime}|. */

/* Include standard types and bool. */
#include <stdint.h>
#include <stdbool.h>

/* Helper primality test: deterministic trial division up to sqrt(n). */
static bool my_is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;
    if (n % 3ULL == 0) return n == 3ULL;
    if (n % 5ULL == 0) return n == 5ULL;
    uint64_t r = 0;
    /* 6k ± 1 wheel */
    uint64_t i = 7;
    while (i <= n / i)
    {
        if (n % i == 0) return false;
        i += 4; /* i = 6k-1 */
        if (i <= n / i && n % i == 0) return false;
        i += 2; /* i = 6k+1 */
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Offset is 1; for n == 0 the value is irrelevant. */
    if (n == 0) return (seq_t)0;

    uint64_t d = 2ULL * (uint64_t)n; /* common difference */
    uint64_t curr = 31ULL;           /* first term of the progression */
    seq_t count = 0;                 /* number of primes found */

    while (true)
    {
        if (!my_is_prime(curr)) break;
        count = count + (seq_t)1;    /* count is small, fits in seq_t */
        curr = curr + d;
    }

    return count;
}
