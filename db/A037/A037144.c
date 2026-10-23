/*
 * A037144  Numbers with at most 3 prime factors (counted with multiplicity).
 * https://oeis.org/A037144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose total number of prime
 *            factors, counted with multiplicity, is at most 3 (i.e. Ω(m) ≤
 *            3). The sequence begins:
 *            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,25,26,...
 *            The implementation builds a growing list of such numbers on
 *            demand. The algorithm...
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the required function signature */
seq_t a(idx_t n);

/* Internal helper that builds the list of numbers ≤ limit with Ω(m) ≤ 3.
   It returns the pointer to the array (the caller owns it) and sets *out_len
   to the number of elements in the array. */
static seq_t *
build_cache(idx_t limit, size_t *out_len)
{
    /* Allocate array for smallest prime factor (spf) up to limit */
    idx_t *spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf) { *out_len = 0; return NULL; }

    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;                         /* i is prime */
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Temporary array to collect qualifying numbers.
       The maximum possible size is limit+1 (including 1). */
    seq_t *tmp = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!tmp) { free(spf); *out_len = 0; return NULL; }

    size_t idx = 0;
    tmp[idx++] = (seq_t)1;                    /* 1 has zero prime factors */

    for (idx_t i = 2; i <= limit; ++i) {
        uint8_t omega = 0;
        idx_t x = i;
        while (x > (idx_t)1 && omega <= 3) {
            ++omega;
            x /= spf[x];
        }
        if (omega <= 3) tmp[idx++] = (seq_t)i;
    }

    free(spf);
    *out_len = idx;
    return tmp;
}

/* Static cache of precomputed sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Public function returning the n-th term of A037144. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for 0, but safe */

    /* Ensure the cache contains at least n elements. */
    if (cache_len < (size_t)n) {
        /* Exponential growth of the bound until enough terms are available. */
        idx_t bound = (idx_t)(n * 4);   /* heuristic start value */
        if (bound < (idx_t)100) bound = 100;

        while (1) {
            size_t new_len = 0;
            seq_t *new_cache = build_cache(bound, &new_len);
            if (!new_cache) return (seq_t)0;    /* allocation failure */

            if (new_len >= (size_t)n) {
                /* Replace the old cache. */
                free(cache);
                cache = new_cache;
                cache_len = new_len;
                break;
            }
            /* Not enough terms: discard and double the bound. */
            free(new_cache);
            bound *= 2;
        }
    }

    return cache[(size_t)n - 1];
}
