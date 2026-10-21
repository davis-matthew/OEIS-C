/*
 * A069360  Number of prime pairs (p,q), p <= q, such that (p+q)/2 = 2*n.
 * https://oeis.org/A069360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1784, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069360 – Number of prime pairs (p,q), p ≤ q, with (p+q)/2 = 2*n.
 *  Formula: a(n) = #{ k | 2*n - k  and  2*n + k  are both prime,
 *                     1 ≤ k ≤ 2*n },   a(1) = 1.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test.  Works for numbers ≤ ~10^12,
 * which is more than enough for this sequence (max value ≈ 4*n). */
static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Return the a(n) term for OEIS sequence A069360. */
seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1; undefined for n==0 */
        return (seq_t)0;

    /* Special case n == 1 */
    if (n == 1) return (seq_t)1;

    /* Work with C integer types for arithmetic; the values fit in 64 bits. */
    uint64_t two_n  = (uint64_t)n * 2ULL;
    uint64_t four_n = two_n * 2ULL;

    uint64_t count = 0;

    for (uint64_t k = 1; k <= two_n; ++k) {
        uint64_t p = two_n - k;   /* 2*n - k   */
        uint64_t q = two_n + k;   /* 2*n + k   */
        if (is_prime_small(p) && is_prime_small(q))
            ++count;
    }

    return (seq_t)count;
}
