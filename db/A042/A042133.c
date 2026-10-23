/*
 * A042133  Denominators of continued fraction convergents to sqrt(591).
 * https://oeis.org/A042133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 331352*a(n-10) - a(n-20) for n > 19
   Initial values: a(0)=1, a(1)=3, a(2)=13, a(3)=16, a(4)=29, a(5)=219,
   a(6)=248, a(7)=467, a(8)=2116, a(9)=6815, a(10)=329236,
   a(11)=994523, a(12)=4307328, a(13)=5301851, a(14)=9609179,
   a(15)=72566104, a(16)=82175283, a(17)=154741387, a(18)=701140831,
   a(19)=2258163880 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The first 20 terms are known a priori.  For n >= 20 we compute
       by recurrence, caching already computed values.  The cache expands
       as needed. */
    static const seq_t init[20] = {
        1, 3, 13, 16, 29, 219, 248, 467, 2116, 6815,
        329236, 994523, 4307328, 5301851, 9609179,
        72566104, 82175283, 154741387, 701140831, 2258163880
    };
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated size of cache */
    static size_t max_comp = 0;   /* largest index already computed */

    if (n < 20) {
        return init[(size_t)n];
    }

    if (cache == NULL) {
        /* Initialise cache with the first 20 terms */
        cap = 20;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i) {
            cache[i] = init[i];
        }
        max_comp = 19;
    }

    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();  /* OOM: abort */
        cache = tmp;
        cap = new_cap;
    }

    if ((size_t)n <= max_comp) {
        return cache[(size_t)n];
    }

    /* Compute terms from max_comp+1 up to n using the recurrence */
    for (size_t i = max_comp + 1; i <= (size_t)n; ++i) {
        seq_t a10 = cache[i - 10];
        seq_t a20 = cache[i - 20];
        cache[i] = a10 * (seq_t)331352 - a20;
    }
    max_comp = (size_t)n;
    return cache[(size_t)n];
}
