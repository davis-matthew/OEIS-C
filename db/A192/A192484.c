/*
 * A192484  Shifts left under XOR-convolution: a(n) = Sum_{k=0..n-1} a(k) XOR
 *          a(n-k-1) for n>1 with a(0)=1, a(1)=2.
 * https://oeis.org/A192484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} (a(k) XOR a(n-1-k)), a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoised implementation that works for arbitrary call order.
   Uses a dynamic array that grows as needed.  All arithmetic is
   performed in the seq_t type supplied by the build system. */
#include <stdlib.h>   /* malloc, realloc, abort */

static void ensure_cache(idx_t needed, seq_t **data, size_t *cap)
{
    if (*cap < needed + 1) {
        size_t new_cap = (*cap == 0) ? 2 : *cap;
        while (new_cap <= needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*data, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* terminate on out‑of‑memory */
        *data = tmp;
        *cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* array of computed terms   */
    static size_t  cap    = 0;     /* allocated capacity        */
    static size_t  size   = 0;     /* number of terms stored    */
    static int     init   = 0;     /* flag for first initialisation */

    /* Make sure the cache can store indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* initialise first two terms on first use */
    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        size = 2;
        init = 1;
    }

    /* compute missing terms up to n */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /* compute half the sum and double it; the middle term (if any)
               is a value XORed with itself, which is zero, so it can be ignored. */
            for (idx_t k = 0; k < i / 2; ++k)
                sum += cache[k] ^ cache[i - 1 - k];
            sum += sum;          /* double the partial sum */
            cache[i] = sum;
        }
        size = n + 1;
    }

    return cache[n];
}
