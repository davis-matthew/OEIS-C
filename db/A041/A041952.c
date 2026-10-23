/*
 * A041952  Numerators of continued fraction convergents to sqrt(499).
 * https://oeis.org/A041952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041952: Numerators of continued fraction convergents to sqrt(499).
   Recurrence: a(n) = 8980*a(n-6) - a(n-12)   (for n ≥ 12)
   Initial values (0 ≤ n < 12):
   a(0)=22, a(1)=45, a(2)=67, a(3)=1452, a(4)=1519, a(5)=4490,
   a(6)=199079, a(7)=402648, a(8)=601727, a(9)=13038915,
   a(10)=13640642, a(11)=40320199.  */

#include <stdlib.h>

#if defined(__cplusplus)
/* C++: use new/delete for object construction/destruction */
#  define ALLOC_SEQ(cap)  new seq_t[(cap)]
#  define FREE_SEQ(p)    delete[] (p)
#else
/* C: use malloc/free; seq_t must be a POD type */
#  define ALLOC_SEQ(cap)  ((seq_t*)malloc((cap)*sizeof(seq_t)))
#  define FREE_SEQ(p)    free(p)
#endif

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* array of computed a(n) */
static idx_t  cache_len  = 0;      /* highest index stored + 1 */
static idx_t  cache_cap  = 0;      /* current allocated capacity */

/* Ensure the cache can provide a(n) for all indices 0 … n inclusive */
static void ensure_cache_size(idx_t n)
{
    /* Initial capacity when first allocating */
    if (cache == NULL) {
        cache_cap = (n < 8) ? 8 : 16;
        cache     = ALLOC_SEQ(cache_cap);
    }

    /* Grow capacity to cover n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = ALLOC_SEQ(new_cap);
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        FREE_SEQ(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first 12 terms if they haven't been set yet */
    if (cache_len < 12) {
        static const seq_t init[12] = {
            (seq_t)22,  (seq_t)45,  (seq_t)67,   (seq_t)1452,
            (seq_t)1519, (seq_t)4490, (seq_t)199079, (seq_t)402648,
            (seq_t)601727, (seq_t)13038915, (seq_t)13640642, (seq_t)40320199
        };
        idx_t end = (cache_len < 12) ? (12) : 12;
        if (n < 12) end = n + 1;
        for (idx_t i = cache_len; i < end; ++i) cache[i] = init[i];
        cache_len = end;
    }

    /* Compute remaining terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i is guaranteed ≥ 12 here because earlier stages set 0…11 */
        cache[i] = (seq_t)8980 * cache[i - 6] - cache[i - 12];
    }
    cache_len = n + 1;
}

/* Public function */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
