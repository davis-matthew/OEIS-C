/*
 * A198648  Number of n X 1 0..6 arrays with values 0..6 introduced in row
 *          major order and each element equal to at least one horizontal or
 *          vertical neighbor.
 * https://oeis.org/A198648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the order‑11 linear recurrence * a(n) =
 *            6*a(n-1) + a(n-2) - 60*a(n-3) + 50*a(n-4) + 226*a(n-5) * -
 *            231*a(n-6) - 416*a(n-7) + 361*a(n-8) + 388*a(n-9) * -
 *            180*a(n-10) - 144*a(n-11) for n > 12, * and the initial values
 *            a(1) … a(12) are given in the problem * statement. The
 *            functio...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 48, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 12 known terms: a(1)..a(12) */
    static const seq_t base[12] = {
        (seq_t)0,   /* a(1) */
        (seq_t)1,   /* a(2) */
        (seq_t)1,   /* a(3) */
        (seq_t)2,   /* a(4) */
        (seq_t)3,   /* a(5) */
        (seq_t)6,   /* a(6) */
        (seq_t)11,  /* a(7) */
        (seq_t)23,  /* a(8) */
        (seq_t)47,  /* a(9) */
        (seq_t)103, /* a(10) */
        (seq_t)226, /* a(11) */
        (seq_t)518  /* a(12) */
    };

    /* Static cache that grows as needed.
     * cache[i] holds a(i+1) for i >= 0. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements already cached */

    /* Initialise cache with the first 12 terms on first call. */
    if (cache_len == 0) {
        cache_len = 12;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = base[i];
    }

    /* If requested value is already cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache to hold up to a(n).  Use a temporary pointer for
     * realloc to avoid unused result warnings. */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;  /* allocation failed */
    cache = tmp;

    /* Compute new terms from cache_len up to n-1 (zero based). */
    for (idx_t k = cache_len; k < n; ++k) {
        /* k is zero based: a(k+1) */
        /* recurrence indices: a(k)   -> cache[k-1]
                               a(k-1) -> cache[k-2]
                               ...
                               a(k-10)-> cache[k-11] */
        seq_t next = (seq_t)6 * cache[k - 1]           /* a(k)   */
                   + cache[k - 2]                      /* a(k-1) */
                   - (seq_t)60 * cache[k - 3]          /* a(k-2) */
                   + (seq_t)50 * cache[k - 4]          /* a(k-3) */
                   + (seq_t)226 * cache[k - 5]         /* a(k-4) */
                   - (seq_t)231 * cache[k - 6]         /* a(k-5) */
                   - (seq_t)416 * cache[k - 7]         /* a(k-6) */
                   + (seq_t)361 * cache[k - 8]         /* a(k-7) */
                   + (seq_t)388 * cache[k - 9]         /* a(k-8) */
                   - (seq_t)180 * cache[k -10]         /* a(k-9) */
                   - (seq_t)144 * cache[k -11];        /* a(k-10) */
        cache[k] = next;
    }

    cache_len = n;
    return cache[n - 1];
}
