/*
 * A080495  a(1) = 1, a(2) = 12, a(n) = smallest multiple of a(n-1) beginning
 *          with the least significant digit of a(n-1).
 * https://oeis.org/A080495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=12,
   for n>2, a(n) is the smallest multiple of a(n-1)
   that begins with the last decimal digit of a(n-1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* return the most significant decimal digit of x (0–9) */
static seq_t leading_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return x;
}

/* ensure cache contains at least n+1 entries (index 0 unused) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    new_cache = new seq_t[new_size];
#else
    new_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
#endif
    if (!new_cache) abort();          /* out of memory */

    /* copy existing values */
    for (idx_t i = 1; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old cache */
#ifndef __cplusplus
    free(cache);
#else
    delete[] cache;
#endif

    cache      = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    ensure_cache(n);

    /* compute all missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i < cache_size && cache[i] != (seq_t)0) continue;

        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)12;
        } else {
            seq_t prev        = cache[i - 1];
            seq_t last_digit  = prev % (seq_t)10;

            idx_t   m   = 2;
            while (1) {
                seq_t candidate = prev * (seq_t)m;
                if (leading_digit(candidate) == last_digit) {
                    cache[i] = candidate;
                    break;
                }
                ++m;
            }
        }
    }
    return cache[n];
}
