/*
 * A224555  Number of (n+2) X 6 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) (n > 8) Initial values: a(1)=172, a(2)=174, a(3)=230,
 *            a(4)=286, a(5)=395, a(6)=551, a(7)=749, a(8)=1041
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 48, u64 n <= 106, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms — index 0 is unused. */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static size_t cache_len = 0;     /* number of allocated elements (cache_len-1 is highest index) */
static idx_t last_computed = 0;  /* highest index computed and stored in cache */

/* Compute terms up to n, using the linear recurrence. */
static void compute_to(idx_t n)
{
    if (n == 0) return;          /* outside the defined range – nothing to compute */

    /* Allocate or grow cache if necessary. */
    if (cache_len == 0) {
        size_t need = (size_t)n + 1;   /* allocate enough for index n */
        if (need < 9) need = 9;        /* at least space for the first 8 terms */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) exit(1);           /* allocation failed */
        cache_len = need;
        /* initialise 1‑based indices */
        cache[0] = 0;                  /* unused */
        cache[1] = (seq_t)172;
        cache[2] = (seq_t)174;
        cache[3] = (seq_t)230;
        cache[4] = (seq_t)286;
        cache[5] = (seq_t)395;
        cache[6] = (seq_t)551;
        cache[7] = (seq_t)749;
        cache[8] = (seq_t)1041;
        last_computed = 8;
    } else if ((size_t)n + 1 > cache_len) {
        /* grow the cache */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_len = new_len;
    }

    if (last_computed >= n) return;  /* already have the answer */

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1]
                   - 6 * cache[i - 2]
                   + 5 * cache[i - 3]
                   - 4 * cache[i - 4]
                   + 3 * cache[i - 5]
                   - cache[i - 6];
    }
    last_computed = n;
}

/* Public function – computes a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    compute_to(n);
    return cache[n];
}
