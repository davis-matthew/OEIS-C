/*
 * A145123  Numbers n such that there exists x in N : (x+1)^3-x^3=19*n^2.
 * https://oeis.org/A145123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 302*a(n+1) - a(n), a(1)=1, a(2)=301
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Memoised recurrence implementation                                   */
/* --------------------------------------------------------------------- */

static seq_t  *a_cache      = NULL;   /* array indexed from 1, a_cache[0] unused */
static size_t  a_cache_cap  = 0;      /* current allocated capacity (max index) */
static size_t  a_cache_last = 0;      /* highest index already filled */
static bool    a_cache_init = false;  /* have the first two terms been set? */

/* enlarge cache if necessary (capacity stored in a_cache_cap) */
static void a_ensure_cap(idx_t n)
{
    if ((size_t)n <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* sequence is defined for n ≥ 1; undefined for n < 1  */
    if (n < 1)
        return 0;

    /* initialise the first two terms on the first call */
    if (!a_cache_init) {
        a_ensure_cap(2);          /* allocate indices 0..2 */
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)301;
        a_cache_last = 2;
        a_cache_init = true;
    }

    /* value already known? */
    if ((size_t)n <= a_cache_last)
        return a_cache[n];

    /* grow the cache to the requested index */
    a_ensure_cap(n);

    /* compute all missing terms via the recurrence */
    for (size_t i = a_cache_last + 1; i <= (size_t)n; ++i)
        a_cache[i] = (seq_t)(302 * a_cache[i - 1] - a_cache[i - 2]);

    a_cache_last = (size_t)n;
    return a_cache[n];
}
