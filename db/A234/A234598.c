/*
 * A234598  Cardinality of the Weyl alternation set corresponding to the
 *          zero-weight in the adjoint representation of the Lie algebra of
 *          so(2n).
 * https://oeis.org/A234598
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4)  (n ≥ 8)
 * Initial values:
 *     a(4) = 9,   a(5) = 18,   a(6) = 35,   a(7) = 82
 *
 * This implementation memoises values in a dynamic array so that calls
 * can arrive in any order.
 */

#include <stdlib.h>

/* Dynamic cache of previously computed terms */
static seq_t *cache = NULL;        /* array of seq_t values   */
static idx_t  last  = 3;           /* highest index computed (∀i≤last cache[i] valid) */

/* Ensure that cache contains values up to index n */
static void ensure(idx_t n)
{
    if (n <= last) return;              /* already computed */

    /* resize the cache to accommodate indices up to n */
    size_t new_cap = (size_t)n + 1;      /* +1 because index n is inclusive */
    if (!cache) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();
    } else if (new_cap > (size_t)(last + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* initialise new entries */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 4)
            cache[i] = (seq_t)9;
        else if (i == 5)
            cache[i] = (seq_t)18;
        else if (i == 6)
            cache[i] = (seq_t)35;
        else if (i == 7)
            cache[i] = (seq_t)82;
        else
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)3 * cache[i-3] + cache[i-4];
    }

    last = n;
}

/* Return a(n) for n ≥ 4; for n < 4 the sequence is undefined */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* undefined below offset, return 0 */
    ensure(n);
    return cache[n];
}
