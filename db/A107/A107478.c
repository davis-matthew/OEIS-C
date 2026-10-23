/*
 * A107478  Slowest increasing sequence where: 1) a(n) cannot contain any "0"
 *          and 2) a(n) must contain one digit of the product of the digits of
 *          a(n-1).
 * https://oeis.org/A107478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_mask, has_zero_digit, prod_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107478: a(1)=1; for n>1, a(n) is the smallest integer >a(n-1) that contains at least one digit from the product of the digits of a(n-1) and contains no zero digit. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_zero_digit.h"
#include "../utils/digit_mask.h"
#include "../utils/prod_of_digits.h"

/* Cache of previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0; /* number of terms stored (indices 1..size) */
static size_t cache_cap = 0; /* allocated elements */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
    {
        /* Ensure capacity for a(1). */
        if (cache_cap < 2)
        {
            seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
            cache = tmp;
            cache_cap = 2;
        }
        cache[1] = (seq_t)1;
        cache_size = 1;
        return cache[1];
    }

    /* Resize cache if needed. */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If terms already computed, return stored value. */
    if (n <= cache_size)
        return cache[n];

    /* Ensure a(1) is present. */
    if (cache_size == 0)
    {
        cache[1] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t prod = prod_of_digits(prev);
        uint16_t prod_mask = digit_mask(prod);

        seq_t cand = prev + 1;
        for (;;)
        {
            if (!has_zero_digit(cand) && (digit_mask(cand) & prod_mask) != 0)
                break;
            ++cand;
        }
        cache[i] = cand;
    }

    cache_size = n;
    return cache[n];
}
