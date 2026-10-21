/*
 * A235169  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235169: Number of (n+1)×2 0..7 arrays with the diagonal/anti‑diagonal
   difference condition 4 and no adjacent equal elements.
   Empirical recurrence (order 9):
       a(n) = 6*a(n‑1) + 37*a(n‑2) – 234*a(n‑3) – 141*a(n‑4)
            + 1290*a(n‑5) + 108*a(n‑6) – 2340*a(n‑7) + 1296*a(n‑9)
   Initial terms (n = 1..9):
       a(1)=268, a(2)=1528, a(3)=8688, a(4)=49464,
       a(5)=281580, a(6)=1603344, a(7)=9129276,
       a(8)=51991032, a(9)=296077680
*/

#include <stddef.h>
#include <stdlib.h>

/* Initial values for n = 1 .. 9 (index 0 unused). */
static const seq_t init[10] = {
    0,          /* dummy 0th element */
    268ULL,
    1528ULL,
    8688ULL,
    49464ULL,
    281580ULL,
    1603344ULL,
    9129276ULL,
    51991032ULL,
    296077680ULL
};

/* 1‑based cache: cache[i] = a(i). */
static seq_t *cache     = NULL;
static idx_t cache_size = 0;       /* number of elements allocated (0 = none) */
static idx_t computed   = 0;       /* highest index computed so far */

/* Ensure that cache contains values up to and including n. */
static void ensure_computed(idx_t n)
{
    /* Allocate (or grow) the cache array if necessary. */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;   /* we use 1‑based indexing */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing values, forward from the last computed index. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 9) {
            /* Base cases are given explicitly. */
            cache[i] = init[i];
        } else {
            /* Compute according to the recurrence.  All arithmetic
               is performed with seq_t, which supports big integers if
               needed. */
            seq_t pos = (seq_t)6   * cache[i-1]
                       + (seq_t)37  * cache[i-2]
                       + (seq_t)1290 * cache[i-5]
                       + (seq_t)108  * cache[i-6]
                       + (seq_t)1296 * cache[i-9];

            seq_t neg = (seq_t)234  * cache[i-3]
                       + (seq_t)141  * cache[i-4]
                       + (seq_t)2340 * cache[i-7];

            cache[i] = pos - neg;   /* guaranteed non‑negative */
        }
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; values below 1 are undefined */
    ensure_computed(n);
    return cache[n];
}
