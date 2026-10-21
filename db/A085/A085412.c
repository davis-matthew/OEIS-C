/*
 * A085412  Numbers n such that concatenation of prime[n] and first digit of
 *          prime[n] is prime.
 * https://oeis.org/A085412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A085412: indices n where prime[n] concatenated with its
   first (most‑significant) digit is again prime.  The value returned
   is a(idx_t n) with 1‑based indexing. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to test primality via deterministic trial division.
   Works for 64‑bit integers suitable for the values that occur in
   this sequence (primes up to ~10^6). */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* Return the most‑significant digit of a positive integer. */
static uint64_t first_digit(uint64_t x)
{
    while (x >= 10U)
        x /= 10U;
    return x;
}

extern seq_t a(idx_t n)
{
    /* Cache of results for memoization. */
    static idx_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* Prime generation state. */
    static uint64_t last_prime = 0;          /* last prime found, 0 if none yet */
    static uint64_t candidate = 2;           /* next candidate to test */
    static idx_t prime_index = 0;            /* 1‑based index of current prime */

    /* Grow result cache as needed. */
    if (seq_len == 0) {
        seq_cap = 32;
        seq = (idx_t *)malloc(seq_cap * sizeof(idx_t));
    }

    while (seq_len < (size_t)n) {
        /* Find next prime. */
        while (!is_prime(candidate))
            ++candidate;
        uint64_t p = candidate;
        ++candidate;                     /* prepare candidate for next iteration */
        ++prime_index;                   /* p is the prime_index‑th prime */

        /* Concatenate prime with its first digit. */
        uint64_t fd = first_digit(p);
        uint64_t concat = p * 10ULL + fd;

        /* If the concatenation is prime, record the index. */
        if (is_prime(concat)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (idx_t *)realloc(seq, seq_cap * sizeof(idx_t));
            }
            seq[seq_len++] = prime_index;
        }
    }

    return (seq_t)seq[n - 1];
}
