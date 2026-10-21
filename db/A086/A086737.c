/*
 * A086737  a(n) = A000217(A000041(n)).
 * https://oeis.org/A086737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(A000041(n)) -- triangular number of the partition
 *            number of n
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 45, u64 n <= 131, u128 n <= 422,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for partition numbers */
static seq_t *p_cache   = NULL;   /* array of p(n) values  */
static size_t p_cache_cap = 0;    /* allocated capacity          */
static idx_t  p_cache_last = 0;   /* last index that has been computed */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_partition_cache(idx_t n)
{
    if ((size_t)(n + 1) <= p_cache_cap)
        return;  /* already enough space */

    size_t new_cap = (size_t)(n + 1) * 2;   /* grow by a factor of 2 for amortised O(n) */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* out of memory */

    if (p_cache != NULL) {
        memcpy(new_cache, p_cache, p_cache_cap * sizeof(seq_t));
        free(p_cache);
    }
    /* initialise the new part of the cache to zero */
    for (size_t i = p_cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    p_cache     = new_cache;
    p_cache_cap = new_cap;
}

/* Compute partition numbers up to index n (inclusive). */
static void compute_partitions_upto(idx_t n)
{
    if (p_cache == NULL) {                 /* first run */
        p_cache       = (seq_t *)malloc(sizeof(seq_t));
        if (!p_cache) abort();
        p_cache[0]    = 1;                 /* p(0) = 1 */
        p_cache_cap   = 1;
        p_cache_last  = 0;
    }

    if (p_cache_last >= n)                 /* already computed */
        return;

    ensure_partition_cache(n);

    /* Compute p(i) for i = p_cache_last+1 ... n */
    for (idx_t i = p_cache_last + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        for (int k = 1; ; ++k) {
            /* Generalised pentagonal numbers */
            idx_t pent1 = (idx_t)k * (3u * k - 1u) / 2u;
            if (pent1 > i) break;

            if (k & 1)
                pos += p_cache[i - pent1];
            else
                neg += p_cache[i - pent1];

            idx_t pent2 = (idx_t)k * (3u * k + 1u) / 2u;
            if (pent2 > i) continue;

            if (k & 1)
                pos += p_cache[i - pent2];
            else
                neg += p_cache[i - pent2];
        }
        p_cache[i] = pos - neg;
    }

    p_cache_last = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Handle base case: a(0) = 1 (since p(0) = 1, triangular(1) = 1) */
    if (n == 0)
        return 1;

    compute_partitions_upto(n);
    seq_t p = p_cache[n];

    /* triangular number of p without intermediate overflow:
       If p is even: (p/2)*(p+1)
       If p is odd:  p*((p+1)/2)      */
    if ((p & 1) == 0)   /* p is even */
        return (p / 2) * (p + 1);
    else                /* p is odd  */
        return p * ((p + 1) / 2);
}
