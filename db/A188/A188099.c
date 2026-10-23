/*
 * A188099  Number of (n+3) X 6 binary arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A188099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) - a(n‑2) + a(n‑4) - 2*a(n‑5) + a(n‑6) + a(n‑8) -
 *            a(n‑9) for n > 13 Initial values: a(1)=1417, a(2)=243, a(3)=486,
 *            a(4)=1421, a(5)=3013, a(6)=3765, a(7)=4041, a(8)=4285,
 *            a(9)=4537, a(10)=4817, a(11)=5317, a(12)=6393, a(13)=9606. The
 *            sequence is computed by memoizing the recurrence,...
 * Range:     u8 none, u16 n <= 20, u32 n <= 65, u64 n <= 154, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free, NULL */

static seq_t *cache = NULL;      /* dynamic array for computed values */
static size_t allocated = 0;     /* allocated size of the array */
static size_t computed = 0;      /* highest n that has been computed (1‑based) */

/* Ensure cache can hold values up to n (1‑based). */
static void compute_to(idx_t n)
{
    size_t n_u = (size_t)n;           /* cast to machine size */
    if (n_u <= computed) return;      /* already computed */

    /* Allocate or grow the array as needed. */
    if (n_u >= allocated) {
        size_t new_alloc = allocated ? allocated * 2 : 16;
        while (new_alloc <= n_u) new_alloc <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_alloc];
#else
        seq_t *new_cache = (seq_t*)malloc(new_alloc * sizeof(seq_t));
#endif
        if (cache) {
            for (size_t i = 0; i <= computed; ++i)
                new_cache[i] = cache[i];
#ifdef __cplusplus
            delete [] cache;
#else
            free(cache);
#endif
        }
        cache = new_cache;
        allocated = new_alloc;
    }

    /* Compute missing terms from computed+1 up to n_u. */
    for (size_t i = computed + 1; i <= n_u; ++i) {
        if (i == 1)             cache[i] = (seq_t)1417;
        else if (i == 2)         cache[i] = (seq_t)243;
        else if (i == 3)         cache[i] = (seq_t)486;
        else if (i == 4)         cache[i] = (seq_t)1421;
        else if (i == 5)         cache[i] = (seq_t)3013;
        else if (i == 6)         cache[i] = (seq_t)3765;
        else if (i == 7)         cache[i] = (seq_t)4041;
        else if (i == 8)         cache[i] = (seq_t)4285;
        else if (i == 9)         cache[i] = (seq_t)4537;
        else if (i == 10)        cache[i] = (seq_t)4817;
        else if (i == 11)        cache[i] = (seq_t)5317;
        else if (i == 12)        cache[i] = (seq_t)6393;
        else if (i == 13)        cache[i] = (seq_t)9606;
        else                     /* recurrence for n > 13 */
            cache[i] = 2*cache[i-1] - cache[i-2] + cache[i-4]
                     - 2*cache[i-5] + cache[i-6] + cache[i-8] - cache[i-9];
    }
    computed = n_u;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0; return 0 */
    compute_to(n);
    return cache[(size_t)n];
}
