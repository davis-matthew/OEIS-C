/*
 * A141844  Expansion of (1+x)*(1+x^2)/((1-x)^2*(1+x+x^2)*(1-4*x)).
 * https://oeis.org/A141844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A141844
 *  Formula : a(0)=1, a(1)=6, a(2)=27, a(3)=113, a(4)=458
 *            a(n) = 5*a(n-1) - 4*a(n-2) + a(n-3) - 5*a(n-4) + 4*a(n-5)   (n>4)
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;        /* dynamic cache of computed terms   */
static size_t seq_cache_size  = 0;      /* allocated number of elements     */
static size_t seq_cache_count = 0;      /* number of terms computed so far  */

/*  Initial terms  */
static const seq_t seq_init[5] = {
    (seq_t)1,
    (seq_t)6,
    (seq_t)27,
    (seq_t)113,
    (seq_t)458
};

seq_t a(idx_t n)
{
    /*  Initialise cache lazily on the first call  */
    if (seq_cache == NULL) {
        seq_cache_size  = 5;
        seq_cache = (seq_t *)malloc(seq_cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i)
            seq_cache[i] = seq_init[i];
        seq_cache_count = 5;
    }

    /*  Return cached value if it exists  */
    if ((size_t)n < seq_cache_count)
        return seq_cache[(size_t)n];

    /*  Grow cache to hold all terms up to n  */
    size_t target = (size_t)n + 1;
    if (seq_cache_size < target) {
        size_t new_size = seq_cache_size ? seq_cache_size << 1 : 8;
        while (new_size < target)
            new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_size = new_size;
    }

    /*  Compute missing terms using the recurrence  */
    for (size_t i = seq_cache_count; i < target; ++i) {
        seq_t a1 = seq_cache[i - 1];
        seq_t a2 = seq_cache[i - 2];
        seq_t a3 = seq_cache[i - 3];
        seq_t a4 = seq_cache[i - 4];
        seq_t a5 = seq_cache[i - 5];

        seq_t val = 5 * a1 - 4 * a2 + a3 - 5 * a4 + 4 * a5;
        seq_cache[i] = val;
    }

    seq_cache_count = target;
    return seq_cache[(size_t)n];
}
