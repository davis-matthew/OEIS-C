/*
 * A141766  A positive integer n is included if both (p-1) and (p+1) divide n
 *          for every prime p that divides n.
 * https://oeis.org/A141766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A141766
 * Numbers n such that for every prime p dividing n, both (p-1) and (p+1) divide n.
 * The definition is checked by a simple test that factorises the candidate
 * with trial division.  No pre‑allocation of prime tables or caching of
 * terms is required; each call computes the requested term from scratch
 * and therefore works for any order of calls. */
#include <stdbool.h>
#include <stdint.h>

/* Test whether a number satisfies the A141766 condition. */
static bool is_valid(uint64_t n)
{
    if (n == 1) return true;  /* 1 has no prime factors */

    uint64_t remaining = n;
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            /* p divides n */
            if (n % (p - 1) != 0 || n % (p + 1) != 0)
                return false;
            /* remove all occurrences of p */
            while (remaining % p == 0) remaining /= p;
        }
    }
    /* remaining > 1 means it is a prime factor */
    if (remaining > 1) {
        uint64_t p = remaining;
        if (n % (p - 1) != 0 || n % (p + 1) != 0)
            return false;
    }
    return true;
}

/* Return the n-th term of A141766 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    uint64_t count   = 0;
    uint64_t candidate = 1;

    while (count < n) {
        if (is_valid(candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }

    return (seq_t)candidate;
}
