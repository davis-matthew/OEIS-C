/*
 * A215458  a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 4*a(n-4) starting 0, 1, 4,
 *          7.
 * https://oeis.org/A215458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 4*a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=4, a(3)=7
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;    /* allocated cache for terms */
static size_t  a_cache_cap = 0;   /* capacity of the allocated cache   */
static idx_t   a_cache_len = 0;   /* highest index that is computed (inclusive) */

/* Ensure the cache can hold indices up to `need` (inclusive). */
static void a_expand_cache(idx_t need)
{
    if (a_cache_cap >= (size_t)need + 1)
        return;

    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
    while (new_cap <= (size_t)need) {
        new_cap *= 2;
    }
    seq_t *new_mem = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (new_mem == NULL) {
        /* Allocation failed – abort as the test framework expects no failures. */
        abort();
    }
    a_cache = new_mem;
    a_cache_cap = new_cap;
}

/* Initialise the first four terms.  This is executed only once. */
static void a_initialise(void)
{
    if (a_cache_len != 0)
        return;
    a_expand_cache(3);           /* need space for indices 0..3 */

    a_cache[0] = (seq_t)0;
    a_cache[1] = (seq_t)1;
    a_cache[2] = (seq_t)4;
    a_cache[3] = (seq_t)7;
    a_cache_len = 4;             /* indices 0..3 are now valid */
}

seq_t a(idx_t n)
{
    a_initialise();

    if (n < a_cache_len)
        return a_cache[n];

    a_expand_cache(n);

    for (idx_t i = a_cache_len; i <= n; ++i) {
        /* Compute term in a way that avoids intermediate underflow: */
        seq_t term1 = 4 * a_cache[i - 1] + 8 * a_cache[i - 3];
        seq_t term2 = 7 * a_cache[i - 2] + 4 * a_cache[i - 4];
        a_cache[i] = term1 - term2;
    }
    a_cache_len = n + 1;  /* all indices up to n are now valid */
    return a_cache[n];
}
