/*
 * A060729  a(n+1) = a(n) + a(n minus the number of terms of the same parity
 *          as n so far).
 * https://oeis.org/A060729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 92, u32 n <= 603, u64 n <= 8265,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 2.
 * For n ≥ 3,
 *   let p = (n-1) mod 2           // parity of the previous index
 *   let c = number of terms among the first n-1 whose values have parity p
 *   then a(n) = a(n-1) + a(n-1 - c)
 * The function memoizes results so that calls may arrive in any order. */

#include <stdlib.h>
#include <stdint.h>

/* static state -------------------------------------------------------- */
static seq_t *cache = NULL;           /* a[0] unused; a[1].. */
static uint32_t *even_counts = NULL;  /* cumulative count of even values */
static size_t cache_cap = 0;          /* current capacity of the arrays */
static idx_t  computed = 0;           /* highest index whose value is known */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for offset 0 */
        return (seq_t)0;

    /* Ensure we have storage for indices 1..n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache   = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        uint32_t *new_even = (uint32_t *)realloc(even_counts,
                                                 new_cap * sizeof(uint32_t));
        if (!new_cache || !new_even) abort();   /* memory failure */
        cache = new_cache;      even_counts = new_even;
        cache_cap = new_cap;
    }

    /* Initialize base values if not already done */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        even_counts[1] = (cache[1] & 1) ? 0 : 1;
        computed = 1;
    }
    if (n >= 2 && computed < 2) {
        cache[2] = (seq_t)2;
        even_counts[2] = even_counts[1] + (((seq_t)2 & 1) ? 0 : 1);
        computed = 2;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t idx = i - 1;                     /* the previous index */

        /* Count of terms with the same parity as idx */
        uint32_t count;
        if ((idx & 1U) == 0U) {                /* idx even → count even values */
            count = even_counts[idx];
        } else {                               /* idx odd  → count odd values */
            count = (uint32_t)(idx - even_counts[idx]);
        }

        /* Position inside the sequence for the recursive term */
        idx_t target_idx = idx - count;        /* guaranteed ≥ 1 for i ≥ 3 */

        /* Compute a[i] */
        cache[i] = cache[idx] + cache[target_idx];

        /* Update cumulative even count for index i */
        if ((cache[i] & 1U) == 0U)
            even_counts[i] = even_counts[idx] + 1U;
        else
            even_counts[i] = even_counts[idx];
    }

    computed = n;
    return cache[n];
}
