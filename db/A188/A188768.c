/*
 * A188768  Number of nX3 binary arrays without the pattern 0 1 0 vertically
 *          or horizontally.
 * https://oeis.org/A188768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 23*a(n-3) + 18*a(n-4) + 5*a(n-5) +
 *            49*a(n-6) - 30*a(n-7) - 87*a(n-8) - 6*a(n-9) + 19*a(n-10) +
 *            8*a(n-11) + 5*a(n-12) - a(n-13) - a(n-14) with initial values
 *            a(1)=7, a(2)=49, a(3)=240, a(4)=1103, a(5)=5357, a(6)=26564,
 *            a(7)=130828, a(8)=641137, a(9)=3143331, a(10)=1542...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms */
static seq_t *cache = NULL;     /* array with indices 0..n */
static idx_t cached_n = 0;     /* maximum n computed so far */
static size_t cached_cap = 0;  /* allocated size of cache */

/* Fixed initial values up to n = 14 */
static const seq_t base[15] = {
    0,
    (seq_t)7,
    (seq_t)49,
    (seq_t)240,
    (seq_t)1103,
    (seq_t)5357,
    (seq_t)26564,
    (seq_t)130828,
    (seq_t)641137,
    (seq_t)3143331,
    (seq_t)15426387,
    (seq_t)75714198,
    (seq_t)371548402,
    (seq_t)1823195326ULL,
    (seq_t)8946675170ULL
};

/* Ensure that cache contains values up to n */
static void ensure_cache(idx_t n)
{
    if (cached_n >= n) return;

    /* Grow allocation if necessary */
    if ((idx_t)(cached_cap) <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (tmp == NULL) abort();  /* allocation failed */
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Initialise base values up to 14 */
    for (idx_t i = 1; i <= 14 && i <= n; ++i) {
        cache[i] = base[i];
    }

    /* Compute new terms starting from max(cached_n+1,15) */
    idx_t start = cached_n + 1;
    if (start < 15) start = 15;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term = (seq_t)5 * cache[i-1]
                   - (seq_t)6 * cache[i-2]
                   + (seq_t)23 * cache[i-3]
                   + (seq_t)18 * cache[i-4]
                   + (seq_t)5  * cache[i-5]
                   + (seq_t)49 * cache[i-6]
                   - (seq_t)30 * cache[i-7]
                   - (seq_t)87 * cache[i-8]
                   - (seq_t)6  * cache[i-9]
                   + (seq_t)19 * cache[i-10]
                   + (seq_t)8  * cache[i-11]
                   + (seq_t)5  * cache[i-12]
                   -       cache[i-13]
                   -       cache[i-14];
        cache[i] = term;
    }

    cached_n = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside the defined offset */
    ensure_cache(n);
    return cache[n];
}
