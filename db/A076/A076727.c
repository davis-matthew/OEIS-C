/*
 * A076727  Primes of the form x^2 + (x+3)^2.
 * https://oeis.org/A076727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 66, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A076727:
 *   primes of the form x² + (x+3)²
 *   Each such prime p can be written uniquely as x² + y² with y−x = 3.
 *   The function returns the nth such prime (offset 1).  If n == 0 it
 *   returns 0; otherwise it uses memoisation so calls in any order
 *   remain fast.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of primes found so far.  cache[i] holds the (i+1)-st prime. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
/* Next x to test.  x starts at 1 because x=0 gives 9, not prime. */
static idx_t next_x = 1;
static bool inited   = false;

/* Ensure the cache can hold at least needed items. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* no term for offset 0 */

    if (!inited) {
        cache = NULL;
        cache_cap = 0;
        cache_len = 0;
        next_x = 1;
        inited = true;
    }

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t x   = (seq_t)next_x;
        seq_t val = (seq_t)2 * x * x + (seq_t)6 * x + 9;  /* x² + (x+3)² */

        if (is_prime_big(val)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        next_x++;
    }

    return cache[n - 1];
}
