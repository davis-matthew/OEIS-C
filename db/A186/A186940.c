/*
 * A186940  Diagonal sums of number triangle A114709.
 * https://oeis.org/A186940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=3, a(3)=6, a(4)=31, a(5)=126 Recurrence:
 *            3*(n+1)*a(n)+(10-17*n)*a(n-1)-6*(n+1)*a(n-2)
 *            +10*(2*n-1)*a(n-3)+9*(1-n)*a(n-4)+(n-2)*a(n-5)=0 =>
 *            a(n)=((17*n-10)*a(n-1)+6*(n+1)*a(n-2)+9*(n-1)*a(n-4)
 *            -10*(2*n-1)*a(n-3)-(n-2)*a(n-5))/(3*(n+1)) (n>=6)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* cached terms */
static idx_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_max = 0;     /* highest computed index */

/* Ensure the cache has at least `needed` slots. */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise cache with the first six known terms. */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache_cap = 64;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    static const seq_t init[6] = {
        (seq_t)1, (seq_t)0, (seq_t)3,
        (seq_t)6, (seq_t)31, (seq_t)126
    };
    for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
    cache_max = 5;
}

seq_t a(idx_t n)
{
    /* direct access for the first six terms */
    if (n < 6) {
        static const seq_t init[6] = {
            (seq_t)1, (seq_t)0, (seq_t)3,
            (seq_t)6, (seq_t)31, (seq_t)126
        };
        return init[n];
    }

    init_cache();                 /* guarantees cache exists */

    if (n > cache_max) {
        ensure_cache_cap(n + 1);
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            idx_t mk = k;             /* keep as index type for safety */

            if (sizeof(seq_t) <= 8) {
                /* 64‑bit integer path: use unsigned __int128 for
                   intermediate arithmetic to avoid overflow */
                unsigned __int128 pos = 0, neg = 0;

                pos += ((unsigned __int128)((17 * mk) - 10)) *
                       (unsigned __int128)cache[k-1];
                pos += ((unsigned __int128)6) * (unsigned __int128)(mk + 1) *
                       (unsigned __int128)cache[k-2];
                pos += ((unsigned __int128)9) * (unsigned __int128)(mk - 1) *
                       (unsigned __int128)cache[k-4];

                neg += ((unsigned __int128)10) * (unsigned __int128)(2 * mk - 1) *
                       (unsigned __int128)cache[k-3];
                neg += ((unsigned __int128)(mk - 2)) *
                       (unsigned __int128)cache[k-5];

                unsigned __int128 numer = pos - neg;
                unsigned __int128 denom = (unsigned __int128)(3 * (mk + 1));

                cache[k] = (seq_t)(numer / denom);
            } else {
                /* arbitrary‑precision path: use seq_t arithmetic only */
                seq_t pos = 0, neg = 0;

                pos += ((seq_t)((17 * mk) - 10)) * cache[k-1];
                pos += ((seq_t)6) * (seq_t)(mk + 1) * cache[k-2];
                pos += ((seq_t)9) * (seq_t)(mk - 1) * cache[k-4];

                neg += ((seq_t)10) * (seq_t)(2 * mk - 1) * cache[k-3];
                neg += ((seq_t)(mk - 2)) * cache[k-5];

                seq_t numer = pos - neg;
                seq_t denom = (seq_t)(3 * (mk + 1));

                cache[k] = numer / denom;
            }
        }
        cache_max = n;
    }
    return cache[n];
}
