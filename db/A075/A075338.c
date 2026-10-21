/*
 * A075338  a(1) = 1, a(2n) = smallest prime obtained by adding a composite
 *          number to a(2n-1). a(2n+1) = the smallest composite number
 *          obtained by adding a prime to a(2n).
 * https://oeis.org/A075338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075338
   a(1) = 1
   For n >= 2 :
     if n is even: a(n) = smallest prime that equals a(n-1) + c,
                   where c is a composite number;
     if n is odd:  a(n) = smallest composite that equals a(n-1) + p,
                   where p is a prime number.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* static cache of computed terms */
static seq_t *cache = NULL;      /* cache[0] holds a(1) */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_len = 0;     /* number of terms currently cached */

/* Ensure cache has room for at least n terms (n >= 1) */
static void ensure_capacity(size_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap <<= 1;
    /* realloc returns void* in C++ – cast explicitly */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for any n >= 1.  The function is memoized. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined below offset, but safe */

    size_t idx = (size_t)n;          /* 1‑based index of the term to compute */

    ensure_capacity(idx);

    /* Seed the cache if empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing terms sequentially */
    for (size_t i = cache_len; i < idx; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sum;
        /* i is 0‑based; term index is i+1 */
        if (((i + 1) & 1) == 0) {            /* even index: need a prime sum */
            uint64_t c = 4;                  /* first composite */
            for (;;) {
                if (!is_prime_small((seq_t)c)) {   /* c is composite */
                    sum = prev + (seq_t)c;
                    if (is_prime_big(sum))
                        break;
                }
                ++c;
            }
            cache[i] = sum;
        } else {                               /* odd index: need a composite sum */
            uint64_t p = 2;                  /* first prime */
            for (;;) {
                if (is_prime_small((seq_t)p)) {     /* p is prime */
                    sum = prev + (seq_t)p;
                    if (!is_prime_big(sum))
                        break;
                }
                ++p;
            }
            cache[i] = sum;
        }
    }

    cache_len = idx;
    return cache[n - 1];
}
