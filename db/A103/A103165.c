/*
 * A103165  Pentagonal 13-gonal numbers.
 * https://oeis.org/A103165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4468995*a(n-2) - 4468995*a(n-4) + a(n-6) for n>6; initial
 *            terms: a(1)=1, a(2)=70, a(3)=6469855, a(4)=314976376,
 *            a(5)=28913745325725, a(6)=1407627555984530
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for previously computed terms */
static seq_t *cache   = NULL;          /* allocated array of terms */
static size_t cache_cap = 0;           /* current allocation size */
static size_t cache_size = 0;          /* number of terms stored */

/* the first six values (0‑based indices) */
static const seq_t init[6] = {
    (seq_t)1ULL,
    (seq_t)70ULL,
    (seq_t)6469855ULL,
    (seq_t)314976376ULL,
    (seq_t)28913745325725ULL,
    (seq_t)1407627555984530ULL
};

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety – never used */

    size_t need = (size_t)n;              /* number of terms required */

    /* grow the cache if necessary */
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        cache_cap = need;
    }

    /* compute any missing terms */
    for (size_t i = cache_size; i < need; ++i) {
        if (i < 6) {
            cache[i] = init[i];
        } else {
            /* apply the recurrence: a(n) = 4468995*a(n-2) - 4468995*a(n-4) + a(n-6) */
            cache[i] = (seq_t)4468995 * cache[i-2]
                     - (seq_t)4468995 * cache[i-4]
                     + cache[i-6];
        }
    }
    cache_size = need;

    /* return the n‑th term (1‑based index) */
    return cache[n-1];
}
