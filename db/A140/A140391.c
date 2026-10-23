/*
 * A140391  Pythagorean triangle side lengths triples with two lengths prime.
 * https://oeis.org/A140391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) enumerates the side lengths of primitive Pythagorean
 *            triples whose two odd lengths are prime. The triples are sorted
 *            by increasing odd leg (which equals 2m-1 for coprime m>n,
 *            n=m-1). Each triple contributes 3 consecutive terms: (even, odd,
 *            hyp). a(1)=4, a(2)=3, a(3)=5, … The function builds...
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 48, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime_big.h"   /* for is_prime_big(seq_t) */

/* static cache of computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of terms stored (>=1 when used) */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* next candidate for m (natural number >1) */
static idx_t next_m = 2;

/* ensure cache has at least `need` terms */
static void grow_cache(size_t need)
{
    while (cache_len < need) {
        /* compute even leg, odd leg, hypotenuse for current m */
        seq_t even_leg = 2 * (seq_t)next_m * (seq_t)(next_m - 1);
        seq_t odd_leg   = (seq_t)next_m * 2 - (seq_t)1;
        seq_t hypotenuse = even_leg + (seq_t)1;  /* 2m(m-1)+1 */

        /* test primality */
        if (is_prime_big(odd_leg) && is_prime_big(hypotenuse)) {
            /* ensure enough space for 3 new terms */
            if (cache_cap < cache_len + 3) {
                size_t newcap = cache_cap ? cache_cap * 2 : 8;
                while (newcap < cache_len + 3) newcap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                assert(tmp); /* abort on allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++]     = even_leg;
            cache[cache_len++]     = odd_leg;
            cache[cache_len++]     = hypotenuse;
        }
        next_m++;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-indexed; guard against zero although not used */
    assert(n > 0);

    /* ensure we have at least n terms */
    if (cache_len < (size_t)n)
        grow_cache((size_t)n);

    return cache[n - 1];
}
