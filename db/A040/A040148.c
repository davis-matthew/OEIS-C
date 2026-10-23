/*
 * A040148  Primes p such that x^4 = 20 has no solution mod p.
 * https://oeis.org/A040148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes p such that x^4 ≡ 20 (mod p) has no solution
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static uint64_t *good = NULL;
static size_t good_cap = 0, good_sz = 0;
static uint64_t candidate = 3;

/* Simple incremental sieve to test primality by trial division. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t f = 3; f * f <= n; f += 2)
        if (n % f == 0) return false;
    return true;
}

/* Grow a dynamic array to at least <needed> elements. */
static void grow_array(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap < needed) new_cap <<= 1;
        void *nptr = realloc(*ptr, new_cap * elem_size);
        if (!nptr) exit(1);
        *ptr = nptr;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Generate good primes until we have at least n of them. */
    while (good_sz < n) {
        /* Find next prime (odd candidates only). */
        while (!is_prime_u64(candidate)) candidate += 2;

        /* Skip prime 5 because 20 ≡ 0 (mod 5). */
        if (candidate != 5) {
            uint64_t exp;
            seq_t r;
            if (candidate % 4 == 1)
                exp = (candidate - 1) / 4;
            else
                exp = (candidate - 1) / 2;

            r = mod_pow((seq_t)20, exp, (seq_t)candidate);
            if (r != (seq_t)1) {
                grow_array(good_sz + 1, &good_cap, (void **)&good, sizeof(uint64_t));
                good[good_sz++] = candidate;
            }
        }

        /* Advance to next odd number. */
        candidate += 2;
    }

    return (seq_t)good[n - 1];
}
