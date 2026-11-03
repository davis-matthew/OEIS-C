/*
 * A142571  Primes congruent to 41 mod 53.
 * https://oeis.org/A142571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 964 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence of primes congruent to 41 modulo 53.  The function
   searches, tests, and caches primes as needed. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  count = 0;

    /* next arithmetic progression index to test */
    static idx_t  next_k = 0;

    if (n == 0)   /* for safety; actual queries start at n >= 1 */
        return (seq_t)0;

    /* generate primes until we have at least n of them */
    while (count < n) {
        idx_t candidate = 41 + 53 * next_k;
        if (is_prime(candidate)) {
            if (count == cap)
                grow_seq(&cache, &cap, count + 1);   /* grow capacity */
            cache[count++] = (seq_t)candidate;
        }
        ++next_k;
    }

    return cache[n-1];
}
