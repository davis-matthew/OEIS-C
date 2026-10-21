/*
 * A085317  Primes which are the sum of three nonzero squares.
 * https://oeis.org/A085317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime (excluding 2) that can be expressed as a
 *            sum of * three non‑zero squares 1 ≤ a ≤ b ≤ c. Equivalently, * a
 *            prime p ≠ 5, 13, 37 and p ≡ 1, 3 or 5 (mod 8)
 * Range:     u8 n <= 36, u16 n <= 4889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_three_nonzero_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_three_nonzero_squares.h"

/* Integer square root for 64‑bit values (floor) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Deterministic primality test for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n == 2 || n == 3)      return true;
    if ((n & 1ULL) == 0)       return false;
    if (n % 3ULL == 0)         return false;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

/* Cached sequence data */
static seq_t *seq_terms = NULL;   /* array of terms */
static size_t seq_len   = 0;       /* number of terms stored */
static size_t seq_cap   = 0;       /* current capacity */
static uint64_t next_candidate = 3; /* next odd number to test */

/* Ensure capacity for at least n elements */
static void seq_ensure_capacity(size_t n)
{
    if (seq_cap >= n) return;
    if (seq_cap == 0) seq_cap = 4;
    while (seq_cap < n) seq_cap <<= 1;
    seq_terms = (seq_t *)realloc(seq_terms, seq_cap * sizeof(seq_t));
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /** Generate terms until we reach the requested index. */
    size_t target = (size_t)n;
    while (seq_len < target) {
        /* Find next prime. */
        while (!is_prime_u64(next_candidate))
            next_candidate += 2;

        /* Check whether it qualifies. */
        if (has_three_nonzero_squares(next_candidate)) {
            seq_ensure_capacity(seq_len + 1);
            seq_terms[seq_len++] = (seq_t)next_candidate;
        }

        /* Move to the next odd candidate. */
        next_candidate += 2;
    }
    return seq_terms[target - 1];
}
