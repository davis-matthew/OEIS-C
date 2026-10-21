/*
 * A082928  If n is prime, a(n) = n+1; if n is even, a(n) = n/2; otherwise
 *          a(n) = n.
 * https://oeis.org/A082928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+1 if n is prime; a(n) = n/2 if n is even; otherwise
 *            a(n) = n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic prime test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* n is at least 1. */
    if (is_prime_uint64((uint64_t)n))
        return (seq_t)n + 1;          /* prime case */
    if ((n & 1) == 0)
        return (seq_t)n / 2;          /* even case */
    return (seq_t)n;                 /* odd non‑prime case */
}
