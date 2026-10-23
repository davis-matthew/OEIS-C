/*
 * A037078  In ternary expansion of n, reading from right to left, digits
 *          occur in order ...,0,1,2,0,1,2,...
 * https://oeis.org/A037078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A037078 :  In ternary expansion of n, reading from right to left,
    digits occur in order ...,0,1,2,0,1,2,...
    Formula (recurrence):
        a(0)=0
        a(1)=1
        a(2)=2
        a(3)=3
        a(4)=7
        a(n)=3*a(n-2)+a(n-3)-3*a(n-5)  for n≥5
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms       */
static size_t cache_cap = 0;         /* current allocation size (index+1)   */
static idx_t  computed_until = (idx_t)-1; /* highest index that has been filled */

/* Ensure the cache holds terms up to and including `n` */
static void
ensure_cache(idx_t n)
{
    /* initialise the base values on first use */
    if (computed_until == (idx_t)-1) {
        size_t init_cap = 5;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return;                     /* allocation failed – caller gets garbage */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 7;
        cache_cap = init_cap;
        computed_until = 4;
    }

    if (computed_until >= n) return;  /* nothing to do */

    /* grow the array if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 5;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failed – caller gets garbage */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms from computed_until+1 up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        seq_t term = 3 * cache[i - 2] + cache[i - 3];
        term -= 3 * cache[i - 5];
        cache[i] = term;
    }
    computed_until = n;
}

seq_t a(idx_t n)
{
    if (n <= 4) {
        static const seq_t base[5] = {0, 1, 2, 3, 7};
        return base[(size_t)n];
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
