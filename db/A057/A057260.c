/*
 * A057260  Numbers n such that n | 9^n + 8^n + 7^n + 6^n + 5^n + 4^n.
 * https://oeis.org/A057260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   mod_pow, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th positive integer n such that
 *          n divides 9^n + 8^n + 7^n + 6^n + 5^n + 4^n.
 * The function returns the value a(n) (1‑based index).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, idx_t exp, seq_t mod); */
#include "../utils/seq_alloc_new_array.h" /* seq_t *seq_alloc_new_array(size_t cap); */
#include "../utils/seq_realloc.h"      /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

static bool is_solution(idx_t cand)
{
    /* compute sum_{k=4}^{9} k^cand (mod cand) */
    seq_t sum = 0;
    const uint64_t bases[6] = {4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 6; ++i) {
        seq_t term = mod_pow((seq_t)bases[i], cand, (seq_t)cand);
        sum += term;
        /* keep sum in [0, cand-1] */
        if (sum >= (seq_t)cand) {
            sum -= (seq_t)cand;
        }
    }
    return sum == 0;
}

seq_t a(idx_t n)
{
    /* static cache of produced terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_test = 1;    /* integer to test next */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1024; /* initial capacity */
        cache = seq_alloc_new_array(cache_cap);
    }

    /* grow cache when needed */
    while (cache_len < n) {
        /* skip even numbers – they never satisfy the property */
        if ((next_test & (idx_t)1) == 0) {
            ++next_test;
            continue;
        }

        if (is_solution(next_test)) {
            /* store the solution */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap * 3) / 2 + 1;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;
        }
        ++next_test;
    }

    return cache[n - 1];
}
