/*
 * A064911  If n is semiprime (or 2-almost prime) then 1 else 0.
 * https://oeis.org/A064911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is a semiprime (product of two primes), else 0
 * Range:     u8 n <= 9547, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple primality test for 64‑bit values using trial division.  */
static inline bool simple_is_prime(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* Return 1 if n is a semiprime, 0 otherwise. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t x = (uint64_t)n;

    /* Prime numbers are not semiprime. */
    if (simple_is_prime(x)) return (seq_t)0;

    /* Find the first divisor. */
    uint64_t p = 0;
    for (uint64_t d = 2ULL; d <= x / d; ++d) {
        if (x % d == 0ULL) {
            p = d;
            break;
        }
    }
    if (p == 0) return (seq_t)0;        /* Should not happen; n is composite. */

    uint64_t q = x / p;                  /* Quotient */

    /* n is semiprime iff both factors are prime. */
    if (simple_is_prime(p) && simple_is_prime(q)) return (seq_t)1;
    return (seq_t)0;
}
