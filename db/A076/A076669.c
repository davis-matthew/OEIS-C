/*
 * A076669  Primes of the form n^2*totient(n)+1 (or A053191(n) + 1).
 * https://oeis.org/A076669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime of the form n^2·φ(n)+1 (checked for n =
 *            1,2,3, …)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 15, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;        /* stored prime values */
static size_t cache_len = 0;       /* number of stored primes */
static size_t cache_cap = 0;       /* current capacity of cache array */
static idx_t next_n = 1;           /* next n to test */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete [] cache;
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    free(cache);
#endif
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1; value undefined for 0 */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute terms until we have at least n primes */
    while (cache_len < n) {
        seq_t nn = (seq_t)next_n;
        seq_t phi = calc_phi(nn);
        seq_t val = nn * nn * phi + 1;

        if (is_prime_big(val)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_n;
    }

    return cache[n - 1];
}
