/*
 * A064434  a(n) = (2*a(n-1) + 1) mod n.
 * https://oeis.org/A064434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*a(n-1) + 1) mod n, with a(1) = 0
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoizes calculated terms in a static array that grows as needed.  */
#include <stdlib.h>

static seq_t *a_cache      = NULL;   /* array of computed terms */
static idx_t  a_cache_size = 0;      /* current allocated size (in terms) */
static idx_t  a_max_computed = 0;    /* largest n for which a has been computed */

/* Grow the cache to hold at least new_size terms. */
static void a_cache_grow(idx_t new_size)
{
    if (new_size <= a_cache_size)
        return;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) { /* unrecoverable error: abort */
        exit(1);
    }
    a_cache = tmp;
    a_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* not used (sequence starts at 1) */
        return 0;

    /* Ensure cache is large enough for n */
    if (n > a_cache_size) {
        a_cache_grow(n);
    }

    /* Initialize a(1) if not already done */
    if (a_max_computed < 1) {
        a_cache[0] = 0;          /* a(1) = 0 */
        a_max_computed = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = a_max_computed + 1; k <= n; ++k) {
        seq_t prev = a_cache[k - 2];          /* a(k-1) */
        seq_t val  = (prev * 2 + 1) % (seq_t)k;  /* a(k) */
        a_cache[k - 1] = val;
    }
    a_max_computed = n;

    return a_cache[n - 1];
}
