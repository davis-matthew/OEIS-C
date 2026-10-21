/*
 * A222113  Goodstein sequence starting with a(1) = 16: to calculate a(n) for
 *          n>1, subtract 1 from a(n-1) and write the result in the hereditary
 *          representation base n, then bump the base to n+1.
 * https://oeis.org/A222113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   Goodstein sequence a(n) with a(1)=16. For n>1: a(n) = value
 *            obtained by writing a(n-1)-1 in hereditary representation base n
 *            and evaluating that representation in base n+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* 1‑based cache of already computed terms */
static seq_t *seq_cache = NULL;   /* array entry 0 unused */
static size_t cache_cap = 0;      /* allocated size (includes index 0) */
static idx_t  max_computed = 0;   /* largest n that has been computed */

/* Extend the cache so that indices 1…n are storeable. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = ((size_t)n + 1) * 2;          /* double to reduce reallocs */
    void *p = realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!p)
        abort();                         /* out of memory */
    seq_cache = (seq_t *)p;
    cache_cap = new_cap;
}

/* Recursive evaluation of a number written in base `base`
   (the hereditary representation’s base) into base `base+1`. */
static seq_t eval_seq(seq_t val, idx_t base)
{
    /* If the value is already a single digit, it is already in hereditary form. */
    if (val < (seq_t)base)
        return val;

    seq_t sum = 0;
    seq_t rem = val;
    seq_t digit_pos = 0;

    while (rem > 0) {
        seq_t digit = rem % (seq_t)base;   /* 0 ≤ digit < base */
        if (digit) {
            seq_t exp = eval_seq(digit_pos, base);   /* hereditary value of the exponent */
            seq_t term = big_pow((seq_t)(base + 1), exp);
            sum += term * digit;
        }
        rem /= (seq_t)base;
        ++digit_pos;
    }
    return sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1 */

    /* Initialise cache once */
    if (seq_cache == NULL)
        ensure_cache(1);

    /* Initialise the first term if not already done */
    if (max_computed < 1) {
        seq_cache[1] = (seq_t)16;
        max_computed = 1;
    }

    /* Compute any missing terms up to n (any call order). */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        ensure_cache(i);                       /* guarantee storage for a[i] */
        seq_t prev = seq_cache[i - 1];
        seq_t decremented = prev - (seq_t)1;
        seq_cache[i] = eval_seq(decremented, i);
    }
    max_computed = n;
    return seq_cache[n];
}
