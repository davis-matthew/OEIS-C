/*
 * A186581  Number of (n+3) X 4 0..2 arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A186581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - 4*a(n-5) * + 2*a(n-6) +
 *            2*a(n-8) - 2*a(n-9) for n > 24 * * Initial values (a(1) …
 *            a(24)): * 43046721, 8824, 38608, 192026, 805791, 1728546, *
 *            2296065, 2604716, 2927679, 3089510, 3313917, 4090548, * 6218902,
 *            9961241, 15014753, 21788652, 31911831, * 45586648,...
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 97, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cache[i] holds a(i+1) */
static size_t cache_size = 0;      /* number of terms currently stored */
static size_t cache_cap   = 0;      /* allocated capacity */

/*-------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined, but harmless */

    /* initialise cache with the first 24 terms on first call */
    if (cache_size == 0) {
        cache_cap = 32;                               /* initial capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;           /* allocation failed */

        static const seq_t init[24] = {
            (seq_t)43046721, (seq_t)8824,     (seq_t)38608,  (seq_t)192026,
            (seq_t)805791,  (seq_t)1728546,   (seq_t)2296065,(seq_t)2604716,
            (seq_t)2927679, (seq_t)3089510,   (seq_t)3313917,(seq_t)4090548,
            (seq_t)6218902, (seq_t)9961241,   (seq_t)15014753,(seq_t)21788652,
            (seq_t)31911831,(seq_t)45586648,  (seq_t)62332637,(seq_t)84072672,
            (seq_t)116767981,(seq_t)164051247,(seq_t)227583882,(seq_t)314652408
        };
        for (size_t i = 0; i < 24; ++i)
            cache[i] = init[i];
        cache_size = 24;
    }

    /* if the requested term is already stored, return it */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* ensure there is enough space */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 32;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (size_t idx = cache_size + 1; idx <= (size_t)n; ++idx) {
        /* idx is 1‑based: a(idx) is at cache[idx-1] */
        seq_t term = 0;
        term += ((seq_t)2) * cache[idx - 2];   /* 2*a(idx-1) */
        term -=               cache[idx - 3];   /* - a(idx-2) */
        term += ((seq_t)2) * cache[idx - 5];   /* + 2*a(idx-4) */
        term -= ((seq_t)4) * cache[idx - 6];   /* - 4*a(idx-5) */
        term += ((seq_t)2) * cache[idx - 7];   /* + 2*a(idx-6) */
        term += ((seq_t)2) * cache[idx - 9];   /* + 2*a(idx-8) */
        term -= ((seq_t)2) * cache[idx -10];   /* - 2*a(idx-9) */
        cache[idx - 1] = term;
    }
    cache_size = n;

    return cache[n - 1];
}
