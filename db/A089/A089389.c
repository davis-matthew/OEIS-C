/*
 * A089389  Sum of the smallest and the largest nontrivial divisor of n or 0
 *          if n is 1 or a prime.
 * https://oeis.org/A089389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 0 if n==1 or n is prime, otherwise a(n) = smallest
 *            non‑trivial divisor of n + the complementary divisor n /
 *            smallest
 * Range:     u8 n <= 507, u16 n <= 9151, u32 n <= 9151, u64 n <= 9151,
 *            u128 n <= 9151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  n is 1‑based; a(1) is the first term. */
#include <stdbool.h>
#include <stdint.h>

/* The type definitions (seq_t and idx_t) are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0 */

    uint64_t v = (uint64_t)n;
    /* handle trivial small numbers */
    if (v == 2 || v == 3)
        return 0;                 /* primes → 0 */

    /* if n is even */
    if ((v & 1ULL) == 0ULL) {
        if (v == 2)
            return 0;             /* prime 2 */
        /* smallest divisor is 2 */
        seq_t other = (seq_t)(v / 2ULL);
        return (seq_t)2ULL + other;
    }

    /* trial division by odd numbers up to sqrt(n) */
    for (uint64_t d = 3UL; d <= v / d; d += 2UL) {
        if (v % d == 0ULL) {      /* found smallest non‑trivial divisor */
            seq_t other = (seq_t)(v / d);
            return (seq_t)d + other;
        }
    }

    /* n is prime (no divisor found) */
    return 0;
}
