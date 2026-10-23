/*
 * A170942  Take the permutations of lengths 1, 2, 3, ... arranged
 *          lexicographically, and replace each permutation with the number of
 *          its fixed points.
 * https://oeis.org/A170942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of fixed points of the n-th permutation in the
 *            concatenation of all lexicographic permutations of 1..k for k =
 *            1,2,3,
 * Range:     u8 n <= 5913, u16 n <= 5913, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


static seq_t *cache      = NULL;   /* computed sequence values    */
static size_t cache_cap  = 0;      /* capacity of the cache       */
static size_t cache_len  = 0;      /* number of elements stored  */
static idx_t  next_k     = 1;      /* next permutation block size to generate */

/* Simple next_permutation implementation for an int array. */
static bool next_permutation_int(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] < a[i]) --j;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        ++l; --r;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside the official range */

    /* Generate permutation blocks until we have at least n values. */
    while (cache_len < n) {
        int k = (int)next_k;                 /* k is small (≤ 8 for n ≤ 10000) */
        int *perm = (int *)malloc(k * sizeof(int));
        if (!perm) exit(EXIT_FAILURE);

        /* initialise permutation 1,2,…,k */
        for (int i = 0; i < k; ++i) perm[i] = i + 1;

        do {
            /* Count fixed points in current permutation */
            idx_t cnt = 0;
            for (int i = 0; i < k; ++i)
                if (perm[i] == i + 1) ++cnt;

            /* Grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(perm); exit(EXIT_FAILURE); }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cnt;
        } while (next_permutation_int(perm, k));

        free(perm);
        ++next_k;                /* move to next block (k+1) */
    }

    return cache[(size_t)n - 1];
}
