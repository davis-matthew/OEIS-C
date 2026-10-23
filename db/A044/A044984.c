/*
 * A044984  Numbers whose base-3 representation contains exactly three 0's and
 *          four 1's.
 * https://oeis.org/A044984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-3 representation contains exactly three 0's and four 1's */
#include <stdlib.h>
#include <stddef.h>

/* next_permutation for arrays of int - standard algorithm */
static int next_permutation(int *a, size_t n)
{
    if (n < 2) return 0;
    size_t i = n - 1;
    while (i > 0 && a[i - 1] >= a[i]) i--;
    if (i == 0) return 0;
    size_t j = n - 1;
    while (a[j] <= a[i - 1]) j--;
    int tmp = a[i - 1];
    a[i - 1] = a[j];
    a[j]   = tmp;
    size_t l = i, r = n - 1;
    while (l < r) {
        int t = a[l];
        a[l] = a[r];
        a[r] = t;
        l++; r--;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cache_cap = 0;  /* current capacity of cache */
    static size_t cache_size = 0;/* number of terms stored */
    static size_t next_L = 7;    /* next ternary length to generate */

    while (cache_size < n) {
        size_t L = next_L++;                     /* current length L */
        size_t k = (L > 7) ? (L - 7) : 0;        /* number of 2's in multiset */

        /* build the multiset: 3 zeros, 4 ones, k twos */
        int *digits = (int*)malloc(L * sizeof(int));
        size_t pos = 0;
        for (size_t i = 0; i < 3; ++i) digits[pos++] = 0;
        for (size_t i = 0; i < 4; ++i) digits[pos++] = 1;
        for (size_t i = 0; i < k; ++i) digits[pos++] = 2;

        /* traverse all unique permutations (lexicographic order) */
        do {
            if (digits[0] != 0) {                      /* skip leading zeros */
                seq_t val = 0;
                for (size_t i = 0; i < L; ++i)
                    val = val * 3 + (seq_t)digits[i];

                /* expand cache if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
                    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = val;
            }
        } while (next_permutation(digits, L));

        free(digits);
    }

    return cache[n - 1];
}
