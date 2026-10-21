/*
 * A202115  Numbers k such that 90*k + 17 is prime.
 * https://oeis.org/A202115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 603, u32 n <= 63, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): nth integer k ≥ 0 such that 90*k+17 is prime (search). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Preallocated cache; 20,000 slots comfortably cover n ≤ 10,000. */
static seq_t k_cache[20000];
static size_t k_cache_len = 0;
static idx_t candidate = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined offset behaviour acceptable */
    while (k_cache_len < (size_t)n) {
        /* Test whether 90*candidate + 17 is prime. */
        seq_t val = (seq_t)90 * (seq_t)candidate + (seq_t)17;
        if (is_prime_big(val)) {
            k_cache[k_cache_len++] = (seq_t)candidate;
        }
        candidate = candidate + 1;       /* advance to next candidate */
    }
    return k_cache[n - 1];
}
