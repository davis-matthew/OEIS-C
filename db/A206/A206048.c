/*
 * A206048  Number of (n+1) X 3 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to one.
 * https://oeis.org/A206048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + a(n-2) - 24*a(n-3) + 15*a(n-4) - 6*a(n-5) +
 *            4*a(n-6); initial values: a(1)=255, a(2)=2310, a(3)=20400,
 *            a(4)=180195, a(5)=1590378, a(6)=14037225
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache[0] dummy, cache[1] holds a(1) */
static size_t cache_cap = 0;         /* allocated size of cache (number of slots) */
static idx_t cache_computed = 0;     /* highest index for which cache[i] is valid */

#ifdef __cplusplus
#define SEQ_CONST(n) seq_t(n)
#else
#define SEQ_CONST(n) ((seq_t)(n))
#endif

/* Ensure the cache has space for indices up to n and initialise the first terms */
static void init_cache(idx_t n)
{
    size_t required = (size_t)(n + 1);   /* need slots 0..n inclusive */
    if (cache_cap < required) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < required)
            new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return;                     /* allocation failed – caller may handle 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_computed < 6) {
        cache[0] = SEQ_CONST(0);
        cache[1] = SEQ_CONST(255);
        cache[2] = SEQ_CONST(2310);
        cache[3] = SEQ_CONST(20400);
        cache[4] = SEQ_CONST(180195);
        cache[5] = SEQ_CONST(1590378);
        cache[6] = SEQ_CONST(14037225);
        cache_computed = 6;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return SEQ_CONST(0);          /* defined only for n ≥ 1, but harmless */
    init_cache(n);
    if (n <= cache_computed)
        return cache[n];

    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        /* recurrence: a(i)=9*a(i-1)+a(i-2)-24*a(i-3)+15*a(i-4)-6*a(i-5)+4*a(i-6) */
        seq_t part1 = cache[i-1] * SEQ_CONST(9);
        seq_t part2 = cache[i-2];
        seq_t part3 = cache[i-3] * SEQ_CONST(24);
        seq_t part4 = cache[i-4] * SEQ_CONST(15);
        seq_t part5 = cache[i-5] * SEQ_CONST(6);
        seq_t part6 = cache[i-6] * SEQ_CONST(4);

        seq_t pos = part1 + part2 + part4 + part6;
        seq_t neg = part3 + part5;
        cache[i] = pos - neg;
    }
    cache_computed = n;
    return cache[n];
}
