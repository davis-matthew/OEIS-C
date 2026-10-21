/*
 * A081842  a(1)=0, a(n)=a(n-1)+4 if n is already in the sequence,
 *          a(n)=a(n-1)+3 otherwise.
 * https://oeis.org/A081842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081842:
   a(1) = 0
   a(n) = a(n‑1) + 4 if n already occurs as a term,
          a(n‑1) + 3 otherwise. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t max_n = 0;

    static unsigned char *present = NULL;
    static size_t pres_cap = 0;

    if (n == 0) /* undefined for n < 1 */
        return (seq_t)0;

    /* create the cache on the first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[1] = (seq_t)0;           /* a(1) = 0 */
        max_n   = 1;

        pres_cap = 128;
        present  = (unsigned char *)calloc(pres_cap, 1);
        present[0] = 1;                 /* value 0 has already appeared */
    }

    /* enlarge the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 128;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* enlarge the present array if necessary; we keep it at least 4×current n */
    size_t required_pres = (size_t)n * 4 + 16;
    if (required_pres > pres_cap) {
        size_t new_cap = pres_cap ? pres_cap : 128;
        while (new_cap < required_pres)
            new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(present, new_cap);
        if (!tmp) abort();
        memset(tmp + pres_cap, 0, new_cap - pres_cap);
        present = tmp;
        pres_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        bool already = (i < pres_cap) && present[(size_t)i];
        seq_t delta = already ? (seq_t)4 : (seq_t)3;
        seq_t cur   = cache[i - 1] + delta;
        cache[i]    = cur;

        /* mark that value cur now appears */
        idx_t val = (idx_t)cur;      /* value fits into idx_t for the tested range */
        if ((size_t)val >= pres_cap) {
            size_t new_cap = pres_cap ? pres_cap : 128;
            while (new_cap <= (size_t)val + 16)
                new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(present, new_cap);
            if (!tmp) abort();
            memset(tmp + pres_cap, 0, new_cap - pres_cap);
            present = tmp;
            pres_cap = new_cap;
        }
        present[val] = 1;
    }

    max_n = n;
    return cache[n];
}
