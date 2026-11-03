/*
 * A049969  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A049969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 3, a(3) = 2;
   For n ≥ 4:
        let p be the unique integer with 2^p < n – 1 ≤ 2^(p+1)
        let m = 2^(p+1) + 2 – n
        a(n) = (a(1)+…+a(n–1)) + a(m)
   (This is OEIS A049969.) */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ilog2.h"
#include "../utils/mem.h"

/* Cached values of the sequence and of the cumulative sums. */
static seq_t *a_cache   = NULL;   /* a_cache[0] unused, a_cache[1] = a(1) … */
static seq_t *sum_cache = NULL;   /* sum_cache[0] = 0, sum_cache[i] = a(1)+…+a(i) */
static idx_t  cache_cap = 0;      /* current allocated capacity (max index) */
static idx_t  max_cached = 0;     /* highest index already computed */

/* Ensure internal arrays have space for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < n)
        new_cap <<= 1;              /* double until enough */

    a_cache   = mem_resize(seq_t, a_cache, (new_cap + 1));
    sum_cache = mem_resize(seq_t, sum_cache, (new_cap + 1));

    /* Keep the cumulative sum base at 0. The realloc may discard old
       memory, but we re‑initialize the zero element each time. */
    if (sum_cache != NULL)
        sum_cache[0] = (seq_t)0;

    cache_cap = new_cap;
}

/* Return the sequence value a(n).  Calls can come in any order. */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined, but return 0 to avoid UB */
        return (seq_t)0;

    /* Grow the cache if needed. */
    if (n > max_cached)
        ensure_capacity(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            a_cache[i] = (seq_t)1;
        }
        else if (i == 2)
        {
            a_cache[i] = (seq_t)3;
        }
        else if (i == 3)
        {
            a_cache[i] = (seq_t)2;
        }
        else
        {
            /* Recurrence for i ≥ 4. */
            unsigned p = ilog2(i - 2);          /* 2^p < i-1 ≤ 2^(p+1) */
            idx_t two_pow = ((idx_t)1) << (p + 1);          /* 2^(p+1) */

            idx_t m = two_pow + (idx_t)2 - i;              /* m = 2^(p+1) + 2 – i */

            a_cache[i] = sum_cache[i - 1] + a_cache[m];
        }

        /* Update the cumulative sum. */
        sum_cache[i] = sum_cache[i - 1] + a_cache[i];
    }

    max_cached = n;
    return a_cache[n];
}
