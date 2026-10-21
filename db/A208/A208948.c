/*
 * A208948  Number of 6-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A208948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 11*a(n-3) - 10*a(n-4) + 11*a(n-5)
 *            - 10*a(n-6) + 5*a(n-7) - a(n-8) Initial values: a(1)=20,
 *            a(2)=264, a(3)=1464, a(4)=5238, a(5)=14430, a(6)=33468,
 *            a(7)=68722, a(8)=128844. (Indexing starts at 1; a(0) is
 *            undefined.)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms (0‑based) */
static size_t cache_size = 0;    /* allocated size of cache   */
static size_t computed_len = 0;  /* number of values already computed */

/* First eight terms (a(1)…a(8)) */
static const seq_t init[8] = {
    (seq_t)20, (seq_t)264, (seq_t)1464, (seq_t)5238,
    (seq_t)14430, (seq_t)33468, (seq_t)68722, (seq_t)128844
};

/* Ensure the cache holds at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_size)
        return;

    /* Allocate new memory of the required size. */
    seq_t *new_mem;
#ifdef __cplusplus
    new_mem = new seq_t[needed];
#else
    new_mem = (seq_t *)malloc(needed * sizeof(seq_t));
#endif
    if (!new_mem) abort();   /* out of memory */

    /* Copy any existing computed values into the new array. */
    for (size_t i = 0; i < computed_len; ++i)
        new_mem[i] = cache[i];

    /* Free the old array. */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_mem;
    cache_size = needed;
}

/* Sequence function.  External linkage. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1 */

    size_t N = (size_t)n;          /* n fits into size_t for our usage */

    /* Make sure the cache can hold N elements. */
    ensure_capacity(N);

    /* Initialise the base values if they are not yet set. */
    if (computed_len < 8) {
        size_t upto = (N < 8 ? N : 8);
        for (size_t i = computed_len; i < upto; ++i)
            cache[i] = init[i];
        computed_len = upto;
    }

    /* Compute any remaining terms using the 8‑term recurrence. */
    for (size_t i = computed_len; i < N; ++i) {
        /* cache[i] corresponds to a(i+1); use i‑1 … i‑8 for a(i)…a(i‑7). */
        cache[i] =  (seq_t)5    * cache[i-1]
                  - (seq_t)10   * cache[i-2]
                  + (seq_t)11   * cache[i-3]
                  - (seq_t)10   * cache[i-4]
                  + (seq_t)11   * cache[i-5]
                  - (seq_t)10   * cache[i-6]
                  + (seq_t)5    * cache[i-7]
                  -               cache[i-8];
    }

    computed_len = N;
    return cache[N-1];
}
