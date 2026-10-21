/*
 * A209882  Smallest k>=0 such that n*(n+1)-(2*k+1) and n*(n+1)+(2*k+1) are
 *          both noncomposite numbers, or -1 if no such k exists.
 * https://oeis.org/A209882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = min { k ≥ 0 | n*(n+1)-(2*k+1) and n*(n+1)+(2*k+1) * are
 *            both non‑composite }
 * Range:     i8 n <= 10, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑composite = 0, 1 or a prime number. */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test for 64‑bit unsigned integers. */
static inline bool is_prime_uint(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Return true if x is non‑composite (x ≤ 1 or prime). */
static inline bool noncomposite_seq(seq_t x)
{
    if (x <= 1) return true;          /* 0 and 1 are non‑composite */
    /* We assume x fits in uint64_t for n up to 10 000. */
    return is_prime_uint((uint64_t)x);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, undefined below */
        return (seq_t)-1;

    /* Triangular number T = n*(n+1).  Use seq_t arithmetic. */
    seq_t T = (seq_t)n * ((seq_t)n + 1);

    /* Search for the smallest k.  The answer is found quickly in practice. */
    for (idx_t k = 0; ; ++k) {
        seq_t off = (seq_t)2 * (seq_t)k + 1;
        seq_t low  = T - off;
        seq_t high = T + off;

        if (noncomposite_seq(low) && noncomposite_seq(high))
            return (seq_t)k;

        /* Prevent an infinite loop in the unlikely event that no solution exists. */
        if (k > 1000000)
            return (seq_t)-1;
    }
}
