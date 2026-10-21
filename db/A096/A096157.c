/*
 * A096157  Numbers whose numbers of odd and even proper divisors differ at
 *          most by 1.
 * https://oeis.org/A096157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 4, all primes, all numbers 4p with prime p, and all
 *            numbers ≡2 (mod 4
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the union: {1,4} ∪ {p prime} ∪ {4p | p prime} ∪ {m | m % 4 == 2}. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* storage for generated sequence terms */
static idx_t cache_len = 0;          /* number of terms stored */
static idx_t cache_cap = 0;          /* current capacity of the cache array */
static idx_t current_num = 1;        /* next integer to test for inclusion */

/* Helper: enlarge cache capacity if needed */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real system we would handle this error.  */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Determine if a candidate integer belongs to the union definition. */
static bool member(idx_t cand) {
    if (cand == 1 || cand == 4)
        return true;
    if (cand % 4 == 2)
        return true;
    if (is_prime(cand))
        return true;
    if (cand % 4 == 0 && is_prime(cand / 4))
        return true;
    return false;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) /* not defined for n=0, but guard */
        return 0;
    while (cache_len < n) {
        if (member(current_num)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)current_num;
        }
        current_num++;
    }
    return cache[n - 1];
}
