/*
 * A142940  Primes congruent to 35 mod 64.
 * https://oeis.org/A142940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes ≡ 35 (mod 64)
 * This function computes the n-th prime p such that p % 64 == 35.
 * It searches for primes in increasing order; no closed form exists.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Ensure that the dynamic array has capacity for at least 'needed' elements. */
static void ensure_capacity(size_t *cap, seq_t **ptr, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    if (new_cap < needed)
        new_cap = needed;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static state to cache computed primes. */
    static seq_t *primes = NULL;
    static size_t count = 0;
    static size_t capacity = 0;
    static uint64_t next_candidate = 35;  /* first number ≡ 35 (mod 64) */

    if (n == 0)            /* undefined below offset – return 0 */
        return 0;

    /* Grow the list until we have at least n primes. */
    while (count < (size_t)n) {
        if (is_prime((idx_t)next_candidate)) {
            ensure_capacity(&capacity, &primes, count + 1);
            primes[count++] = (seq_t)next_candidate;
        }
        next_candidate += 64;
    }

    return primes[n - 1];
}
