/*
 * A045695  Number of ternary words of length n (beginning 0) with
 *          autocorrelation function 2^(n-1)+1.
 * https://oeis.org/A045695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for OEIS A045695
 *
 *  a(1) = 0
 *  a(2) = 1
 *  a(2n)   = 3*a(2n-1) + 2*a(n)   for n >= 2
 *  a(2n-1) = 3*a(2n-2) - a(n)     for n >= 2
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;

/* Ensure cache can hold values up to index 'needed' (inclusive) */
static void ensure_cache(idx_t needed)
{
    if ((idx_t)needed + 1 <= (idx_t)cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity << 1 : 64;
    while (new_cap <= (size_t)needed + 1)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();

    /* Zero‑initialize newly allocated slots – valid for POD or trivial types */
    if (new_cache != cache)
        memset(new_cache + cache_capacity,
               0,
               (new_cap - cache_capacity) * sizeof(seq_t));

    cache = new_cache;
    cache_capacity = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;          /* a(1) = 0 */
    if (n == 2)
        return (seq_t)1;          /* a(2) = 1 */

    ensure_cache(n);

    if (cache[n] != 0)            /* value already computed */
        return cache[n];

    seq_t result;
    if ((n & 1) == 0) {           /* n is even: a(2k) */
        idx_t k = n >> 1;
        seq_t a_prev = a(n - 1);       /* a(2k-1) */
        seq_t a_k    = a(k);           /* a(k)    */
        result = a_prev * 3;
        result += a_k * 2;
    } else {                      /* n is odd: a(2k-1) */
        idx_t k = (n + 1) >> 1;
        seq_t a_prev = a(n - 1);       /* a(2k-2) */
        seq_t a_k    = a(k);           /* a(k)    */
        result = a_prev * 3;
        result -= a_k;
    }

    cache[n] = result;
    return result;
}
