/*
 * A129623  Numbers which are the product of a non-palindrome and its
 *          reversal, where leading zeros are not allowed.
 * https://oeis.org/A129623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 11195, u128 n <= 11195,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 8993 (2026-11-03)
 * Helpers:   is_palindrome_base, mem, reverse_digits_idx
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

#include <stddef.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

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
        if (is_palindrome_base((seq_t)x, 10))
            continue;                  /* skip palindromes           */
        idx_t revx = reverse_digits_idx(x);
        seq_t prod = (seq_t)x * (seq_t)revx;
        tmp[count++] = prod;
    }

    /* Sort the collected products. */
    mem_qsort(tmp, count, sizeof(seq_t), cmp_seq_t);

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
