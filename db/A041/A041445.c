/*
 * A041445  Denominators of continued fraction convergents to sqrt(238).
 * https://oeis.org/A041445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 23326*a(n-8) - a(n-16) for n>15;
   a(0)=1, a(1)=2, a(2)=5, a(3)=7, a(4)=103, a(5)=110, a(6)=323, a(7)=756,
   a(8)=23003, a(9)=46762, a(10)=116527, a(11)=163289, a(12)=2402573,
   a(13)=2565862, a(14)=7534297, a(15)=17634456 */
#include <stdlib.h>

/* Initial 16 terms of the sequence */
static const seq_t init[16] = {
    1ULL, 2ULL, 5ULL, 7ULL, 103ULL, 110ULL, 323ULL, 756ULL,
    23003ULL, 46762ULL, 116527ULL, 163289ULL,
    2402573ULL, 2565862ULL, 7534297ULL, 17634456ULL
};

/* Memoisation storage */
static seq_t *cache = NULL;   /* cache[0..done_upto] are valid */
static size_t cache_size = 0; /* allocated size of cache */
static idx_t done_upto = 15;  /* largest index computed */

/* Return the denominator of the nth continued‑fraction convergent
   to sqrt(238).  Works for any order of calls. */
seq_t a(idx_t n)
{
    /* Fetch from stored initial values if n < 16 */
    if (n < (idx_t)16)
        return init[(size_t)n];

    /* Initialise cache on first entry */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – caller will handle */
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init[i];
        done_upto = 15;
    }

    /* Grow cache if required */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    if (n > done_upto) {
        for (size_t i = (size_t)done_upto + 1; i <= (size_t)n; ++i) {
            /* i-8 and i-16 are valid because i >= 16 here */
            cache[i] = (seq_t)23326 * cache[i - 8] - cache[i - 16];
        }
        done_upto = (size_t)n;
    }

    return cache[(size_t)n];
}
