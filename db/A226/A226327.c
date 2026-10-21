/*
 * A226327  Sum of digits of n equals sum of digits of its antisigma
 *          (A024816).
 * https://oeis.org/A226327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 768, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A226327:
   Numbers n whose decimal digit sum equals the digit sum of
   antisigma(n) = n*(n+1)/2 – sigma(n) (sum of divisors of n).
   a(n) returns the nth such number (1‑based offset). */

#include <stddef.h>     /* size_t */
#include <stdint.h>     /* uint64_t, uint8_t */
#include <stdlib.h>     /* exit() */

#include "../utils/seq_alloc.h"    /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t*,size_t,size_t) */

/* Compute the sum of decimal digits of a 64‑bit unsigned integer. */
static seq_t digit_sum_u64(uint64_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* Compute the sum of decimal digits of a seq_t value. */
static seq_t digit_sum_seq(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Compute the sum of divisors of n (n is idx_t). */
static uint64_t sigma_u64(idx_t n)
{
    uint64_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += i;
            idx_t twin = n / i;
            if (twin != i)
                sum += twin;
        }
    }
    return sum;
}

/* Static cache that remembers already found terms. */
static seq_t *cache = NULL;     /* array of found terms */
static size_t cache_cap = 0;    /* current capacity of array */
static size_t cache_len = 0;    /* number of terms stored */
static idx_t probe_next = 1;    /* next integer to test */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* offset is 1; undefined for n == 0 */
        return (seq_t)0;

    /* Extend cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        idx_t x = probe_next++;

        /* Digit sum of x. */
        seq_t dsum_x = digit_sum_u64((uint64_t)x);

        /* Sum of divisors of x. */
        uint64_t sigma32 = sigma_u64(x);

        /* Antisigma: T - sigma. */
        seq_t T = (seq_t)x * ((seq_t)x + 1) / 2;
        seq_t antisigma = T - (seq_t)sigma32;

        /* Digit sum of antisigma. */
        seq_t dsum_a = digit_sum_seq(antisigma);

        /* If the digit sums match, record this term. */
        if (dsum_x == dsum_a) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) exit(1);        /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)x;
        }
    }

    return cache[n - 1];
}
