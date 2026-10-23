/*
 * A181246  Number of n X 3 binary matrices with no 2 X 2 block having four
 *          1's.
 * https://oeis.org/A181246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 10*a(n-2) - 5*a(n-3); Initial values: a(1)=8,
 *            a(2)=57, a(3)=417
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] holds a(1) */
static idx_t  cache_size  = 0;       /* size of allocated cache (in terms) */
static idx_t  computed_up = 0;       /* highest n computed so far (>=0) */

static void ensure_capacity(idx_t n)
{
    if (n <= cache_size) return;
    /* allocate a little extra space to reduce reallocs */
    size_t new_size = (size_t)n + 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();               /* memory allocation failed */
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1; return 0 */

    /* Ensure we have a cache big enough */
    ensure_capacity(n);

    /* Initialize base values if not already present */
    if (computed_up < 1 && n >= 1) {
        cache[0] = (seq_t)8;
        computed_up = 1;
    }
    if (computed_up < 2 && n >= 2) {
        cache[1] = (seq_t)57;
        computed_up = 2;
    }
    if (computed_up < 3 && n >= 3) {
        cache[2] = (seq_t)417;
        computed_up = 3;
    }

    /* Compute further terms up to n if needed */
    for (idx_t i = computed_up + 1; i <= n; ++i) {
        /* recurrence: a(i) = 6*a(i-1) + 10*a(i-2) - 5*a(i-3) */
        seq_t term = (seq_t)6 * cache[i - 2]        /* a(i-1) */
                   + (seq_t)10 * cache[i - 3]       /* a(i-2) */
                   - (seq_t)5  * cache[i - 4];     /* a(i-3) */
        cache[i - 1] = term;
    }
    computed_up = n;

    return cache[n - 1];
}
