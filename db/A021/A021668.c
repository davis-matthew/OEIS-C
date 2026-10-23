/*
 * A021668  Decimal expansion of 1/664.
 * https://oeis.org/A021668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of decimal expansion of 1/664
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed digits */
    static size_t cap = 0;        /* current capacity of cache */
    static size_t computed = 0;   /* number of digits already computed (so next digit index is computed) */
    static unsigned int rem = 1;  /* remainder after computing last digit (initially 1 for 1/664) */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Ensure the cache is large enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing digits up to n if necessary */
    while (computed <= (size_t)n) {
        seq_t digit = (seq_t)((rem * 10U) / 664U);
        rem = (rem * 10U) % 664U;
        cache[computed] = digit;
        ++computed;
    }

    return cache[(size_t)n];
}
