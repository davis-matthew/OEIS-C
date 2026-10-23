/*
 * A045874  a(1)=1, a(n) is the smallest multiple of n that begins with
 *          a(n-1).
 * https://oeis.org/A045874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A045874: a(1)=1; for n>1, a(n) is the smallest multiple of n
 * that begins with a(n-1).
 */
#include <stdlib.h>
#include <stdint.h>

/* You may need to include the utility providing pow10() */
#include "../utils/pow10.h"

static seq_t *cache = NULL;      /* memoisation table (indexing from 1) */
static idx_t cache_size = 0;     /* current allocated size */

static void resize_cache(idx_t new_cap)
{
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    /* copy old contents */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    /* initialise the rest to 0 */
    for (idx_t i = cache_size; i < new_cap; ++i)
        new_cache[i] = 0;
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cache_size = new_cap;
}

static void ensure_cache(idx_t n)
{
    if (n >= cache_size) {
        idx_t new_cap = cache_size ? cache_size * 2 : 2;
        while (new_cap <= n) new_cap *= 2;
        resize_cache(new_cap);
    }
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* Sequence only defined for n>=1 */

    ensure_cache(n);

    if (cache[1] == 0) cache[1] = 1;  /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        seq_t prev = cache[i - 1];
        seq_t pow10 = 1;
        const seq_t multi = (seq_t)i;

        while (1) {
            seq_t low  = prev * pow10;
            seq_t high = (prev + 1) * pow10;
            /* smallest multiple of i in [low, high) */
            seq_t q = (low + multi - 1) / multi;        /* ceil(low / i) */
            seq_t candidate = q * multi;
            if (candidate < high) {
                cache[i] = candidate;
                break;
            }
            pow10 *= 10;  /* next higher order */
        }
    }
    return cache[n];
}
