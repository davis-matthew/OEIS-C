/*
 * A096423  a(1)=a(2)=1, a(n) = (a(n-1)+1)*(a(n-2)+1) for n > 2.
 * https://oeis.org/A096423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+1)*(a(n-2)+1), a(1)=a(2)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static idx_t capacity = 0;    /* allocated array size */
static idx_t computed = 0;    /* number of cached terms */

/* enlarge the cache so that it contains at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity)
        return;
    idx_t new_cap = capacity == 0 ? needed : capacity * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);     /* allocation failure */
    cache = tmp;
    capacity = new_cap;
}

/* initialise a(1) and a(2) if they are not already cached */
static void init_base(void)
{
    if (computed >= 2)
        return;
    ensure_capacity(2);
    cache[0] = 1;  /* a(1) */
    cache[1] = 1;  /* a(2) */
    computed = 2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behaviour below offset does not matter */

    idx_t required = n > 2 ? n : 2;   /* need at least space for a(1) and a(2) */
    ensure_capacity(required);
    init_base();

    /* compute missing terms up to a(n) */
    while (computed < n) {
        /* cache[computed] corresponds to a(computed+1) */
        seq_t x = cache[computed-1] + 1;   /* a(computed) + 1 */
        seq_t y = cache[computed-2] + 1;   /* a(computed-1) + 1 */
        cache[computed] = x * y;
        ++computed;
    }

    return cache[n-1];   /* a(n) */
}
