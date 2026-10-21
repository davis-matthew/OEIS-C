/*
 * A231295  Number of (n+1) X (1+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal, vertical and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) – 4*a(n-4) Initial values: a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=12 Uses static cache to handle any call
 *            order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache[i] holds a(i)   */
static idx_t  cache_len = 0;      /* allocated size (>= max index + 1) */
static idx_t  max_computed = 0;   /* highest n for which a(n) is set */

/* Resize cache so that indices 0..n are usable. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len)
        return;

    idx_t newlen = cache_len ? (cache_len << 1) : 8;
    if (newlen < n + 1)
        newlen = n + 1;
    while (newlen <= n)
        newlen <<= 1;

    /* realloc may return void*; cast to seq_t* for C/C++ compatibility. */
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!tmp)
        abort();                     /* realloc failed */

    /* Zero the new elements to avoid garbage when using arithmetic. */
    for (idx_t i = cache_len; i < newlen; ++i)
        tmp[i] = 0;

    cache     = tmp;
    cache_len = newlen;
}

/* Initialise the first four terms (used only once). */
static void init_base(void)
{
    if (max_computed >= 4)
        return;
    ensure_cache(4);
    cache[1] = 1;
    cache[2] = 2;
    cache[3] = 4;
    cache[4] = 12;
    max_computed = 4;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* OEIS offset is 1, but return 0 for safety */
        return 0;

    ensure_cache(n);
    init_base();

    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence uses up to a(i-4); i >= 5 guaranteed by init_base. */
            cache[i] = 2 * cache[i - 1]
                     + 3 * cache[i - 2]
                     - 4 * cache[i - 4];
        }
        max_computed = n;
    }
    return cache[n];
}
