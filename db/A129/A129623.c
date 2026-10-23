/*
 * A129623  Numbers which are the product of a non-palindrome and its
 *          reversal, where leading zeros are not allowed.
 * https://oeis.org/A129623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 8993, u128 n <= 8993,
 *            big any n
 * Helpers:   cmp_seq_t, free_seq_array, is_palindrome, rev, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129623: numbers which are the product of a non‑palindrome and its
 * reversal, with no leading zeros.  The function generates all
 * products x * rev(x) for 1 ≤ x ≤ limit (excluding palindromes and
 * numbers ending in 0), sorts them, deduplicates, and returns the
 * n‑th smallest in ascending order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/rev.h"
#include "../utils/is_palindrome.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* Helper: sort and deduplicate a temporary array of seq_t values. */
static void build_upto(idx_t limit, idx_t *len, seq_t **arr)
{
    /* Allocate a temporary array using the helper that constructs
     * seq_t objects correctly for the arbitrary‑precision build. */
    seq_t *tmp = seq_alloc_array(limit);
    if (!tmp) abort();

    idx_t count = 0;
    for (idx_t x = 1; x <= limit; ++x) {
        if (x % 10 == 0)                /* skip numbers ending in 0   */
            continue;
        if (is_palindrome((seq_t)x, 10))
            continue;                  /* skip palindromes           */
        idx_t revx = rev(x);
        seq_t prod = (seq_t)x * (seq_t)revx;
        tmp[count++] = prod;
    }

    /* Sort the collected products. */
    qsort(tmp, count, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate in place and allocate the final array. */
    seq_t *final = seq_alloc_array(count);
    if (!final) abort();
    idx_t uniq = 0;
    for (idx_t i = 0; i < count; ++i) {
        if (i == 0 || cmp_seq_t(&tmp[i - 1], &tmp[i]) != 0) {
            final[uniq++] = tmp[i];
        }
    }

    free_seq_array(tmp);
    *len = uniq;
    *arr = final;
}

/* Public function for the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for 0, but avoid UB */

    /* Static cache. */
    static idx_t cache_len = 0;
    static seq_t *cache_arr = NULL;
    static idx_t build_limit = 200000;      /* start range */

    /* Build cache first time. */
    if (!cache_arr) {
        build_upto(build_limit, &cache_len, &cache_arr);
    }

    /* Extend cache until it contains at least n terms. */
    while (n > cache_len) {
        build_limit *= 2;
        free_seq_array(cache_arr);
        build_upto(build_limit, &cache_len, &cache_arr);
    }

    return cache_arr[n - 1];
}
