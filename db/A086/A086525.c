/*
 * A086525  a(n) = a(( a(n-2))*(1-mod(n,2))+a(n-1)*(mod(n,2))) + a((n -
 *          a(n-2))*(1-mod(n,2))+(n-a(n-1))*(mod(n,2))).
 * https://oeis.org/A086525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a( a(n-2)*(1-mod(n,2)) + a(n-1)*mod(n,2) ) + a( (n -
 *            a(n-2))*(1-mod(n,2)) + (n - a(n-1))*mod(n,2) ) Initial values:
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *a_cache = NULL;      /* memoised terms (1‑based) */
static idx_t a_cache_max = 0;      /* highest computed index */

/*
 * Compute a(n) with arbitrary‑precision or fixed‑width integers.
 * The function works for any build where seq_t is a POD type.
 * For non‑POD seq_t implementations the build must provide a
 * suitable allocator (e.g. seq_alloc_new_array) which is not used
 * here, but the explicit cast to (seq_t*) satisfies both C and C++.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1, safe default */

    /* Grow the cache if necessary */
    if (n > a_cache_max) {
        idx_t new_size = n + 1;              /* +1 to keep 1‑based indexing */
        seq_t *tmp = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp && new_size) abort();      /* allocation failed */
        a_cache = tmp;
    }

    /* Compute missing values iteratively */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        if (i == 1)
            a_cache[1] = 1;
        else if (i == 2)
            a_cache[2] = 1;
        else {
            /* choose previous term of the same parity */
            idx_t prev = (i & 1) ? (i - 1) : (i - 2);

            seq_t prev_val = a_cache[prev];     /* a(prev) */
            idx_t idx1 = (idx_t)prev_val;       /* a(a(prev)) index */

            seq_t term1 = a_cache[idx1];        /* a(a(prev)) */
            idx_t idx2 = i - idx1;              /* a(i - a(prev)) index */
            seq_t term2 = a_cache[idx2];        /* a(i - a(prev)) */

            a_cache[i] = term1 + term2;
        }
    }

    a_cache_max = n;
    return a_cache[n];
}
