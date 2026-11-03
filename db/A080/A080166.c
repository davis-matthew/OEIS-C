/*
 * A080166  Primes having initial digits "11" in their binary representation.
 * https://oeis.org/A080166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime whose binary representation starts with
 *            "11"
 * Range:     u8 n <= 26, u16 n <= 3212, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 70 terms through n = 9146 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Test whether the two most significant bits of a 64‑bit integer are 1. */
static inline bool has_initial_11(uint64_t p)
{
    /* p is guaranteed to be ≥ 3 when this function is called. */
    int msb = 63 - __builtin_clzll(p);          /* position of the highest set bit (0‑based) */
    /* The next significant bit is at position msb-1.  It must be set. */
    return ((p >> (msb - 1)) & 1ULL) != 0ULL;
}

/* Simple deterministic primality test for 64‑bit values. */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;

    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* Return the n‑th member of sequence A080166 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    idx_t found = 0;
    uint64_t candidate = 3; /* the smallest prime that satisfies the condition */

    for (;;)
    {
        if (has_initial_11(candidate) && is_prime_u64(candidate))
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        /* Skip even numbers; 2 is not in the sequence. */
        candidate += 2ULL;
    }
}
