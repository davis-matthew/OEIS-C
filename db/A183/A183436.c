/*
 * A183436  Number of n X 3 binary arrays with every 1 having exactly one
 *          king-move neighbor equal to 1.
 * https://oeis.org/A183436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 8*a(n-3) - a(n-4) - 2*a(n-5) -
 *            2*a(n-6) for n > 6; initial values: a(1)=3, a(2)=13, a(3)=29,
 *            a(4)=89, a(5)=273, a(6)=751
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for abort, though seq_t is defined elsewhere */

static seq_t *cache = NULL;      /* array 0..n, cache[0] unused */
static size_t cache_len = 0;     /* allocated length (>= 1 if cache not NULL) */
static size_t cache_max = 0;     /* highest index already computed */
static const seq_t base[7] = {0,3,13,29,89,273,751};

seq_t a(idx_t n)
{
    /* offset: a(0) is undefined, return 0 for safety */
    if (n == 0) return 0;

    /* For the first 6 terms, just return the hard‑coded values. */
    if (n <= 6) return base[(size_t)n];

    /* Ensure cache array is large enough. */
    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
    if (cache_len < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        cache = tmp;
        cache_len = needed;
    }

    /* Initialise the first six entries unconditionally. */
    cache[1] = base[1];
    cache[2] = base[2];
    cache[3] = base[3];
    cache[4] = base[4];
    cache[5] = base[5];
    cache[6] = base[6];

    /* Update cache_max to reflect that 1..6 are available. */
    if (cache_max < 6) cache_max = 6;

    /* Compute any missing terms up to n. */
    if (n > cache_max) {
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1]
                     + 3*cache[i-2]
                     + 8*cache[i-3]
                     - cache[i-4]
                     - 2*cache[i-5]
                     - 2*cache[i-6];
        }
        cache_max = (size_t)n;
    }

    return cache[(size_t)n];
}
