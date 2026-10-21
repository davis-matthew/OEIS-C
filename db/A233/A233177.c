/*
 * A233177  Number of 4Xn 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally, diagonally or
 *          antidiagonally, top left element zero, and 1 appearing before 2 3
 *          and 4, and 2 appearing before 3 in row major order (unlabeled
 *          6-colorings with no clashing color pairs).
 * https://oeis.org/A233177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233177: Number of 4×n 0..5 arrays with the stated constraints.
 * Recurrence (for n>8):
 *   a(n) = 64*a(n-1) + 3328*a(n-2) - 77824*a(n-3) - 1441792*a(n-4)
 *          + 28311552*a(n-5) + 33554432*a(n-6) - 1073741824*a(n-7)
 * Initial values:
 *   a(1)=48, a(2)=896, a(3)=96256, a(4)=7938048,
 *   a(5)=736362496, a(6)=65265467392, a(7)=5896922988544,
 *   a(8)=528589678182400.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoised results; index 0 unused */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated size (>= index+1)   */
static idx_t cache_last = 0;          /* highest n that has been computed */

/* initialise the cache with the first eight terms */
static void init_cache(void)
{
    cache_cap = 9;                       /* indices 0..8 inclusive */
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;                  /* allocation failure – the function will return 0 */

    cache[0] = (seq_t)0ULL;              /* unused, keep defined */
    cache[1] = (seq_t)48ULL;
    cache[2] = (seq_t)896ULL;
    cache[3] = (seq_t)96256ULL;
    cache[4] = (seq_t)7938048ULL;
    cache[5] = (seq_t)736362496ULL;
    cache[6] = (seq_t)65265467392ULL;
    cache[7] = (seq_t)5896922988544ULL;
    cache[8] = (seq_t)528589678182400ULL;
    cache_last = 8;
}

/* extend the cache to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > (size_t)n) return;    /* already large enough */

    size_t needed = (size_t)(n + 1);
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) { cache = tmp; cache_cap = new_cap; }
    else { cache_cap = 0; }                /* on failure the function will return 0 */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0ULL;        /* undefined for n<1 */

    if (!cache) init_cache();              /* lazy allocation */
    if (!cache) return (seq_t)0ULL;        /* allocation failed */

    if (n <= cache_last) return cache[n];

    ensure_cache_size(n);
    if (!cache) return (seq_t)0ULL;        /* allocation failed */

    /* compute successive terms until n is reached */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        /* accumulate positive contributions */
        seq_t pos =
            cache[i-1] * (seq_t)64ULL           +
            cache[i-2] * (seq_t)3328ULL         +
            cache[i-5] * (seq_t)28311552ULL    +
            cache[i-6] * (seq_t)33554432ULL;

        /* accumulate negative contributions */
        seq_t neg =
            cache[i-3] * (seq_t)77824ULL       +
            cache[i-4] * (seq_t)1441792ULL     +
            cache[i-7] * (seq_t)1073741824ULL;

        cache[i] = pos - neg;                /* result is always non‑negative */
    }

    cache_last = n;
    return cache[n];
}
