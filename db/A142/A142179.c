/*
 * A142179  Primes congruent to 34 mod 39.
 * https://oeis.org/A142179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the n‑th prime p ≡ 34 (mod 39). */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t capacity = 0;
static idx_t count = 0;
static idx_t next_candidate = 34;

static void ensure_capacity(size_t needed)
{
    if (needed <= capacity) return;
    size_t newcap = capacity ? capacity * 2 : 64;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    capacity = newcap;
}

static void grow_until(idx_t n)
{
    while (count < n) {
        if (is_prime(next_candidate)) {
            ensure_capacity(count + 1);
            cache[count++] = (seq_t)next_candidate;
        }
        next_candidate += 39;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    grow_until(n);
    return cache[n - 1];
}
