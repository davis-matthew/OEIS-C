/*
 * A224758  a(2)=7; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, digit_len, free_seq_array, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 7;
   for n > 1, a(n) is the smallest positive integer m such that
   a(n-1) + m = (a(n-1) followed by the leading decimal digit of m).
   Equivalent test: for the smallest d ∈ {1,…,9} with
   m = 9·a(n-1) + d having leading digit d, then a(n)=m. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/digit_len.h"   /* int digit_len(seq_t); */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t); */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t); */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t*); */

/* cache for memoisation */
static seq_t *cache = NULL;    /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;   /* allocated length of cache array   */
static idx_t cache_len = 0;    /* number of terms currently stored */

/* resize cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;
    seq_t *new_arr = alloc_seq_array(new_cap);
    if (!new_arr) {
        /* allocation failed; terminate */
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];
    free_seq_array(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* compute the next term given a(n‑1) */
static seq_t next_term(seq_t prev)
{
    for (int d = 1; d <= 9; ++d) {
        seq_t m = prev * (seq_t)9 + (seq_t)d;
        int len = digit_len(m);
        seq_t pow = pow10((idx_t)(len - 1));
        seq_t hd = m / pow;                /* leading digit, 1‑9 */
        if ((unsigned int)hd == (unsigned int)d)
            return m;
    }
    /* Definition guarantees a solution exists */
    return (seq_t)0;
}

/* ensure that a(1)…a(n) are in the cache */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    if (cache_len == 0) {
        grow_cache((size_t)n);
        cache[0] = (seq_t)7;   /* a(1) = 7 */
        cache_len = 1;
    } else if (cache_cap < (size_t)n) {
        grow_cache((size_t)n);
    }

    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t m = next_term(prev);
        cache[i] = m;
    }
    cache_len = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* sequence defined only for n ≥ 1 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
