/*
 * A010066  a(n+1) = a(n) + sum of digits in base 5 representation of a(n).
 * https://oeis.org/A010066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(n+1) = a(n) + sum_of_digits_in_base5(a(n)) for n ≥ 0
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* dynamic array holding computed terms   */
static size_t cache_cap = 0;    /* allocated capacity in elements         */
static idx_t  cache_len = 0;    /* number of terms already computed      */

/* Ensure the cache can hold at least `needed` elements. */
static void
ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Sum of the digits of x in base 5.  The argument is of type seq_t. */
static seq_t
digit_sum_base5(seq_t x)
{
    seq_t sum = 0;
    const seq_t five = 5;
    while (x > 0) {
        sum += x % five;
        x   /= five;
    }
    return sum;
}

/* OEIS A010066 */
seq_t a(idx_t n)
{
    /* Initialise the first term once. */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Extend the cache up to index n if necessary. */
    if ((idx_t)cache_len <= n) {
        size_t need = (size_t)(n + 1);
        ensure_capacity(need);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev + digit_sum_base5(prev);
        }
        cache_len = (idx_t)need;
    }

    return cache[n];
}
