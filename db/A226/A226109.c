/*
 * A226109  Triangular numbers t such that t - 4, t - 2, t + 2, t + 4 are four
 *          primes.
 * https://oeis.org/A226109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A226109
   a(n) = the nth triangular number t such that
          t-4, t-2, t+2, t+4 are all prime.
   The function builds the sequence lazily and memoises results. */
#include <stdlib.h>

/* Utility for primality testing on seq_t values.
   The helper is provided by the environment: */
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;     /* memoised sequence values     */
static idx_t cache_len = 0;     /* number of stored terms        */
static size_t cache_cap = 0;    /* allocated capacity            */
static idx_t next_k = 1;        /* next triangular index to test */

static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is defined for n ≥ 1 */

    /* Generate terms until we have at least n entries. */
    while (cache_len < n) {
        idx_t k = next_k++;            /* current triangular index   */
        seq_t kk = (seq_t)k;
        seq_t t  = kk * (kk + 1) / 2;  /* triangular number */

        if (t <= (seq_t)4) continue;   /* would underflow for t-4 */

        seq_t t1 = t - (seq_t)4;
        seq_t t2 = t - (seq_t)2;
        seq_t t3 = t + (seq_t)2;
        seq_t t4 = t + (seq_t)4;

        if (is_prime_big(t1) && is_prime_big(t2) &&
            is_prime_big(t3) && is_prime_big(t4)) {
            ensure_cache_capacity();
            cache[cache_len++] = t;
        }
    }
    return cache[n - 1];
}
