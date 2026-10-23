/*
 * A154727  Triangle read by rows in which row n lists all the pairs of prime
 *          numbers that are equidistant from n, or only n if there is no such
 *          pair, as shown below in the example.
 * https://oeis.org/A154727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1715, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: flatten the triangle whose row n lists all primes
 * p < q such that p + q = 2n (distinct prime pairs equidistant from n);
 * if no such pair exists the row contains only n.
 * The sequence a(n) is the list of all numbers in this triangle
 * read row‐by‐row. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Ensure the cache has capacity for at least 'need' elements. */
static void ensure_cache_capacity(seq_t **ptr, size_t *cap_ptr, size_t need)
{
    size_t cap = *cap_ptr;
    if (need <= cap) return;
    size_t new_cap = cap ? cap : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!newp) abort();              /* out of memory */
    *ptr = newp;
    *cap_ptr = new_cap;
}

/* Small insertion sort for the short rows. */
static void insertion_sort(seq_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* Global cache for the flattened triangle. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static idx_t last_row = 0;      /* highest processed row */

/* Extend the cache so that it contains at least 'nterms' terms. */
static void extend_cache_to(idx_t nterms)
{
    while (cache_size < (size_t)nterms) {
        ++last_row;                      /* new row number */
        idx_t r = last_row;
        size_t row_cap = 64;
        seq_t *row = (seq_t *)malloc(row_cap * sizeof(seq_t));
        if (!row) abort();

        size_t row_len = 0;
        bool has_pair = false;
        idx_t two_r = r << 1;          /* 2 * r */

        for (idx_t p = 2; p < r; ++p) {
            if (is_prime(p) && is_prime(two_r - p)) {
                /* add the distinct pair (p, q) with q = 2r - p > r */
                if (row_len + 2 > row_cap) {
                    row_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(row, row_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    row = tmp;
                }
                row[row_len++]     = (seq_t)p;
                row[row_len++]     = (seq_t)(two_r - p);
                has_pair = true;
            }
        }
        if (!has_pair) {
            /* no pair: the row consists of the single number r */
            if (row_len + 1 > row_cap) {
                row_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(row, row_cap * sizeof(seq_t));
                if (!tmp) abort();
                row = tmp;
            }
            row[row_len++] = (seq_t)r;
        }

        insertion_sort(row, row_len);

        /* append the row into the global cache */
        ensure_cache_capacity(&cache, &cache_cap, cache_size + row_len);
        for (size_t i = 0; i < row_len; ++i) {
            cache[cache_size + i] = row[i];
        }
        cache_size += row_len;

        free(row);
    }
}

/* Octet: sequence entry at index n (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined but safe */
    extend_cache_to(n);
    return cache[n - 1];
}
