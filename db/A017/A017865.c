/*
 * A017865  Expansion of 1/(1-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16).
 * https://oeis.org/A017865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 61, u32 n <= 113, u64 n <= 215,
 *            u128 n <= 420, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A017865
 *  Formula: a(n) = a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+
 *           a(n-13)+a(n-14)+a(n-15)+a(n-16)   for n > 16
 *  Initial values:
 *      a(0)=1
 *      a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0
 *      a(7)=1, a(8)=1, a(9)=1, a(10)=1, a(11)=1, a(12)=1, a(13)=1
 *      a(14)=2, a(15)=3, a(16)=4
 */

#include <stdlib.h>

#if defined(__cplusplus)
#  define MEM_ALLOC(num, type) new type[num]
#  define MEM_INIT(arr, num, type)                                 \
     do {                                                          \
         for (idx_t i = 0; i < (num); ++i) (arr)[i] = (type)0;     \
     } while (0)
#else
#  define MEM_ALLOC(num, type) ((type*)malloc((num)*sizeof(type)))
#  define MEM_INIT(arr, num, type)                                 \
     do {                                                          \
         for (idx_t i = 0; i < (num); ++i) (arr)[i] = (type)0;     \
     } while (0)
#endif

static seq_t *cache = NULL;     /* array of computed terms            */
static idx_t  cache_size = 0;   /* highest index currently computed + 1 */
static idx_t  cache_cap  = 0;   /* allocated length of cache           */

/* Ensure cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    /* grow until capacity > n */
    idx_t new_cap = (cache_cap == 0) ? 17 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_cache;
    if (cache_cap == 0) {
        new_cache = MEM_ALLOC(new_cap, seq_t);
        if (!new_cache) exit(1);                     /* allocation failed */
        MEM_INIT(new_cache, new_cap, seq_t);
    } else {
        new_cache = MEM_ALLOC(new_cap, seq_t);
        if (!new_cache) exit(1);
        /* copy old values */
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        /* init the rest to 0 */
        MEM_INIT(new_cache + cache_size, new_cap - cache_size, seq_t);
    }
    /* free old cache if allocated (only in C++) */
#if defined(__cplusplus)
    delete [] cache;
#endif
    cache = new_cache;
    cache_cap = new_cap;

    /* initialise the first 17 values only once */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= 6; ++i) cache[i] = (seq_t)0;
        cache[7] = (seq_t)1;
        cache[8] = (seq_t)1;
        cache[9] = (seq_t)1;
        cache[10] = (seq_t)1;
        cache[11] = (seq_t)1;
        cache[12] = (seq_t)1;
        cache[13] = (seq_t)1;
        cache[14] = (seq_t)2;
        cache[15] = (seq_t)3;
        cache[16] = (seq_t)4;
        cache_size = 17;
    }
}

/* Public function computing a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_size) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* linear recurrence over parts 7..16 */
        sum += cache[i - 7];
        sum += cache[i - 8];
        sum += cache[i - 9];
        sum += cache[i - 10];
        sum += cache[i - 11];
        sum += cache[i - 12];
        sum += cache[i - 13];
        sum += cache[i - 14];
        sum += cache[i - 15];
        sum += cache[i - 16];
        cache[i] = sum;
    }
    cache_size = n + 1;
    return cache[n];
}
