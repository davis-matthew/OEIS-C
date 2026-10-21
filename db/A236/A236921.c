/*
 * A236921  Number of n-permutations which fix at least one odd prefix.
 * https://oeis.org/A236921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=0, a(n) = Σ_{odd k ≤ n-1} (k! – a(k)) * (n – k)! for n
 *            ≥ 2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types: seq_t and idx_t supplied by the build. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*) */

static seq_t *fact_arr    = NULL;   /* factorial 0! … n!   */
static seq_t *a_cache     = NULL;   /* a(n) values 0…n     */
static idx_t  cache_cap   = 0;      /* capacity (max index + 1) */

/* Ensure the fact_arr and a_cache arrays can hold values up to index max_n. */
static void ensure_capacity(idx_t max_n) {
    if (cache_cap > max_n) return;          /* already sufficient */

    size_t new_cap = (size_t)(max_n + 1);    /* new size (indices 0…max_n) */

    /* Allocate new arrays. */
    seq_t *new_fact = alloc_seq_array(new_cap);
    seq_t *new_a    = alloc_seq_array(new_cap);

    /* Copy old contents. */
    for (idx_t i = 0; i < cache_cap; ++i) {
        new_fact[i] = fact_arr[i];
        new_a[i]    = a_cache[i];
    }

    /* Initialise new entries. */
    for (idx_t i = cache_cap; i <= max_n; ++i) {
        if (i == 0 || i == 1) {
            new_fact[i] = (seq_t)1;
            new_a[i]    = (seq_t)0;          /* a(0) and a(1) are zero   */
        } else {
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
            new_a[i]    = (seq_t)0;          /* mark as uncomputed       */
        }
    }

    /* Free old arrays and commit new ones. */
    if (fact_arr) free_seq_array(fact_arr);
    if (a_cache)  free_seq_array(a_cache);

    fact_arr  = new_fact;
    a_cache   = new_a;
    cache_cap = (idx_t)max_n;
}

/* Compute a(n) using the recurrence, memoising results. */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    ensure_capacity(n);

    /* If already computed, just return. */
    if (a_cache[n] != (seq_t)0) return a_cache[n];

    /* Compute a(i) for all i from 2 up to n (increasing order). */
    for (idx_t i = 2; i <= n; ++i) {
        if (a_cache[i] != (seq_t)0) continue;   /* already known */

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i - 1; k += 2) {         /* odd k only */
            seq_t term = (fact_arr[k] - a_cache[k]) * fact_arr[i - k];
            sum += term;
        }
        a_cache[i] = sum;
    }
    return a_cache[n];
}
