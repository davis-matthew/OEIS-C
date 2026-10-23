/*
 * A123654  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+809)^2 = y^2.
 * https://oeis.org/A123654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1618 for n > 6; * initial values:
 *            a(1)=0, a(2)=264, a(3)=1491, * a(4)=2427, a(5)=3811, a(6)=10764
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 28, u64 n <= 65, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array holding computed values */
static size_t cache_size = 0;   /* current allocated size of cache   */
static idx_t  cached_to = 0;    /* highest index that has been computed */

/* initialise cache with the first six values */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_size = 7;                     /* indices 0..6 used */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                /* cannot recover from OOM */
        cache[0] = 0;                       /* unused, kept zero */
        cache[1] = 0;
        cache[2] = 264;
        cache[3] = 1491;
        cache[4] = 2427;
        cache[5] = 3811;
        cache[6] = 10764;
        cached_to = 6;
    }
}

/* grow cache to at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size)
        return;

    size_t new_size = cache_size * 2;
    while (new_size <= (size_t)n)
        new_size *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                  /* cannot recover from OOM */
    cache = tmp;
    cache_size = new_size;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= 6)
        return cache[n];

    if (n > cached_to) {
        ensure_cache(n);
        for (idx_t i = cached_to + 1; i <= n; ++i) {
            seq_t val = cache[i - 3] * (seq_t)6;
            val -= cache[i - 6];
            val += (seq_t)1618;
            cache[i] = val;
        }
        cached_to = n;
    }
    return cache[n];
}
