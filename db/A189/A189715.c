/*
 * A189715  Numbers k such that A156595(k-1) = 0; complement of A189716.
 * https://oeis.org/A189715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189715: numbers whose squarefree part is congruent
   modulo 9 to 1, 4, 6 or 7.  A(n) is the n‑th such integer
   (offset 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* Return true iff the squarefree part of x is ≡ 1,4,6 or 7 (mod 9). */
static bool is_good(uint64_t x)
{
    int prod = 1;              /* product of odd‑exponent primes mod 9 */
    uint64_t y = x;

    /* trial division for prime factors up to sqrt(y) */
    for (uint64_t p = 2; p * p <= y; ++p)
    {
        if (y % p == 0)
        {
            int odd = 0;              /* whether exponent of p is odd */
            while (y % p == 0)
            {
                y /= p;
                odd ^= 1;
            }
            if (odd)
                prod = (prod * (int)(p % 9)) % 9;
        }
    }
    /* y now is 1 or a prime larger than sqrt(original x) */
    if (y > 1)                     /* still an odd exponent */
        prod = (prod * (int)(y % 9)) % 9;

    return (prod == 1 || prod == 4 || prod == 6 || prod == 7);
}

/* external (non‑static) function required by the build */
seq_t a(idx_t n)
{
    /* offset is 1: a(1)=1; undefined behaviour for n==0 */
    assert(n >= 1);

    /* cached list of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_candidate = 0;   /* last tested integer */

    /* enlarge cache until it holds at least n terms */
    while (cache_len < (size_t)n)
    {
        /* ensure capacity */
        if (cache_cap <= cache_len)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();                 /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* find next term */
        do
        {
            ++last_candidate;
        } while (!is_good(last_candidate));

        cache[cache_len++] = (seq_t)last_candidate;
    }

    return cache[n - 1];
}
