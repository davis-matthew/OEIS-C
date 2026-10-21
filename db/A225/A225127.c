/*
 * A225127  Convolutory inverse of the nonprimes.
 * https://oeis.org/A225127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = -∑_{k=2}^{n} r(k) * a(n–k+1), where
 *            r(k) is the k‑th nonprime integer (1,4,6,8,9,10,…)
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 48, i128 n <= 98,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* memoised values of a(n) (0‑based: a(1) is stored at a[0]) */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* list of nonprime integers, 1‑based: r[1]=1, r[2]=4, … */
static uint64_t *nonprimes = NULL;
static size_t np_cap = 0;
static size_t np_len = 0;
static uint64_t cur_val = 1;            /* last integer examined when building nonprimes */

/* grow the a_cache array to at least 'need' entries */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* grow the nonprimes array to at least 'need' entries */
static void grow_nonprimes(size_t need)
{
    if (need <= np_cap) return;
    size_t new_cap = np_cap ? np_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    nonprimes = (uint64_t *)realloc(nonprimes, new_cap * sizeof(uint64_t));
    np_cap = new_cap;
}

/* ensure we have at least 'need' nonprime values stored */
static void ensure_nonprimes(size_t need)
{
    if (np_len >= need) return;
    grow_nonprimes(need);
    while (np_len < need) {
        if (cur_val == 1) {                 /* 1 is a nonprime */
            nonprimes[np_len++] = 1;
            cur_val++;
            continue;
        }
        if (!is_prime(cur_val))
            nonprimes[np_len++] = cur_val;
        cur_val++;
    }
}

/* ensure that the a_cache contains at least 'need' terms */
static void ensure_cache(size_t need)
{
    if (cache_len >= need) return;
    grow_cache(need);

    /* initialise first term */
    if (cache_len == 0) {
        a_cache[0] = 1;                    /* a(1) = 1 */
        cache_len = 1;
    }

    /* make sure nonprime list is long enough */
    ensure_nonprimes(need);

    /* compute missing a(n) values */
    for (size_t m = cache_len + 1; m <= need; ++m) {
        seq_t sum = 0;
        for (size_t k = 2; k <= m; ++k) {
            seq_t prod = (seq_t)nonprimes[k - 1] * a_cache[m - k];
            sum += prod;
        }
        a_cache[m - 1] = -sum;
    }
    cache_len = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, undefined */
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return a_cache[idx - 1];
}
