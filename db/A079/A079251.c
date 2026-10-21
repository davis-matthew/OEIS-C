/*
 * A079251  Complement of A079000.
 * https://oeis.org/A079251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n-1)+2, b(0)=0 b(2n) = 2b(n)+1+3[n>1] b(2n+1) =
 *            2b(n)+1+5[n>0]
 * Range:     u8 n <= 81, u16 n <= 9975, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for computed b(k) values */
static seq_t *b_cache = NULL;
static idx_t cache_size = 0;

/* ensure that b_cache has at least (n+1) elements (indices 0..n) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;
    idx_t new_size = cache_size ? cache_size << 1 : 1;
    while (new_size <= n)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return;               /* out of memory – caller will fail gracefully */
    if (b_cache) {
        memcpy(new_cache, b_cache, cache_size * sizeof(seq_t));
        free(b_cache);
    }
    /* initialise new entries to 0 (0 is safe sentinel for uncomputed values) */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    b_cache = new_cache;
    cache_size = new_size;
}

/* recursively compute b(k) with memoisation */
static seq_t get_b(idx_t k)
{
    if (k == 0) {            /* base case */
        ensure_cache(1);
        b_cache[0] = 0;
        return 0;
    }

    if (k < cache_size && b_cache[k] != 0)   /* already computed (all non‑zero) */
        return b_cache[k];

    idx_t m = k >> 1;        /* floor(k/2)   for both even and odd k   */
    seq_t bm = get_b(m);
    seq_t res;

    if ((k & 1) == 0) {      /* k is even: k = 2m  */
        res = bm * 2 + 1;
        if (m > 1)          /* add 3 only if n>1 (i.e. m>1) */
            res += 3;
    } else {                 /* k is odd: k = 2m + 1 */
        res = bm * 2 + 1;
        if (m > 0)          /* add 5 only if n>0 (i.e. m>0) */
            res += 5;
    }

    ensure_cache(k + 1);
    b_cache[k] = res;
    return res;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first listed term */
    if (n == 0)             /* undefined for n<1; return 0 to avoid UB */
        return 0;
    return get_b(n - 1) + 2;
}
