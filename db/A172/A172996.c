/*
 * A172996  Primes of form (3^n+7)/2.
 * https://oeis.org/A172996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^k + 7)/2 where k is the n-th exponent giving a prime,
 *            a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow3.h"
#include "../utils/is_prime_big.h"

/* Cache of previously computed terms */
static seq_t *cache = NULL;
static size_t cache_count = 0;
static size_t cache_capacity = 0;

/* Helper to grow the cache as needed */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* Handle out-of-memory */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Computes the n-th term of A172996 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but guard against */
    /* Return cached value if available */
    if ((size_t)n <= cache_count)
        return cache[n - 1];

    /* Extend the cache up to n terms */
    /* We keep track of the exponent we have examined so far.
       Since the sequence grows extremely fast, the number of
       exponents needed is very small (<= 15 for n up to 10,000). */
    idx_t exp = 1;
    idx_t prime_cnt = 0;
    idx_t cache_index = 0; /* index in cache where next term will be stored */
    /* Reset cache if we re-examine from the beginning (in case
       earlier calls were made with smaller n). */
    cache_count = 0;

    while (prime_cnt < n) {
        seq_t val = (pow3(exp) + (seq_t)7) / (seq_t)2;
        if (is_prime_big(val)) {
            ++prime_cnt;
            ensure_cache_capacity(cache_index + 1);
            cache[cache_index++] = val;
        }
        ++exp;
    }
    cache_count = cache_index;
    return cache[n - 1];
}
