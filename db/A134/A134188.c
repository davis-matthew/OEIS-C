/*
 * A134188  a(0)=1. a(n) = the sum of the terms of the sequence (from among
 *          terms a(0) through a(n-1)) which equal any "non-isolated divisors"
 *          of (2n). A divisor, k, of n is non-isolated if (k-1) or (k+1) also
 *          divides n.
 * https://oeis.org/A134188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the sum of all earlier terms equal to any non‑isolated
 *            divisor of 2n (a divisor k is non‑isolated if (k‑1) or (k+1)
 *            also divides 2n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only what is strictly necessary for the implementation. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values.  */
static seq_t *seq_cache = NULL;      /* array holding a(i) values.  */
static idx_t seq_cache_cap = 0;       /* current capacity of seq_cache.  */
static idx_t seq_cache_len = 0;       /* number of terms that have been computed.  */

/* Arrays that keep, for every distinct value that has appeared in the
   sequence so far, its occurrence count.  */
static seq_t *distinct_vals = NULL;   /* distinct values that have appeared.  */
static size_t *distinct_counts = NULL;/* counts of those values.  */
static size_t distinct_cap = 0;        /* current capacity of the two arrays.  */
static size_t distinct_len = 0;        /* number of distinct values stored.  */

/* Ensure the distinct arrays have the capacity needed for a new value. */
static void grow_distinct(void)
{
    if (distinct_len >= distinct_cap) {
        size_t new_cap = distinct_cap ? distinct_cap * 2 : 16;
        distinct_vals   = (seq_t *)realloc(distinct_vals,
                                           new_cap * sizeof(seq_t));
        distinct_counts = (size_t *)realloc(distinct_counts,
                                            new_cap * sizeof(size_t));
        distinct_cap = new_cap;
    }
}

/* Return the number of times `v` has occurred among previously
   computed terms of the sequence (indices 0 … n‑1). */
static size_t get_count(seq_t v, idx_t up_to)
{
    size_t cnt = 0;
    for (idx_t i = 0; i < up_to; ++i)
        if (seq_cache[i] == v) ++cnt;
    return cnt;
}

/* Record a new occurrence of value `v` after it has been added to the
   sequence cache.  */
static void add_value(seq_t v)
{
    /* Update the count in the distinct table.  */
    for (size_t i = 0; i < distinct_len; ++i) {
        if (distinct_vals[i] == v) {
            ++distinct_counts[i];
            return;
        }
    }
    /* If we get here the value is new.  */
    grow_distinct();
    distinct_vals[distinct_len]   = v;
    distinct_counts[distinct_len] = 1;
    ++distinct_len;
}

/* Compute and cache all terms up to (and including) `n`.  This routine
   can be called in any order of `n`.  It therefore ensures that terms
   may be requested out of sequence.  */
static void ensure_cache(idx_t n)
{
    if (seq_cache_len > n)  /* already computed.  */
        return;

    /* Allocate or grow the main cache as needed.  */
    if (seq_cache == NULL) {
        seq_cache_cap = 1024;
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
    }
    while (seq_cache_cap <= n + 1) {
        seq_cache_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
    }

    /* Initialise the very first term a(0)=1 and update the distinct
       structure.  */
    if (seq_cache_len == 0) {
        seq_cache[0] = (seq_t)1;
        add_value(seq_cache[0]);
        seq_cache_len = 1;
    }

    /* Compute terms from seq_cache_len up to n. */
    for (idx_t idx = seq_cache_len; idx <= n; ++idx) {
        seq_t value = (seq_t)0;
        if (idx != 0) {
            idx_t m = idx * 2;          /* we need divisors of 2i.  */
            /* enumerate all positive divisors of m.  */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d != 0) continue;

                /* process divisor d */
                idx_t k = d;
                bool non_isolated = false;
                if (k > 1 && m % (k - 1) == 0) non_isolated = true;
                else if (m % (k + 1) == 0)      non_isolated = true;
                if (non_isolated) {
                    size_t cnt = get_count((seq_t)k, idx);
                    value += (seq_t)cnt * (seq_t)k;
                }

                /* if d and m/d are distinct, process the complementary divisor */
                idx_t dk = m / d;
                if (dk != d) {
                    k = dk;
                    non_isolated = false;
                    if (k > 1 && m % (k - 1) == 0) non_isolated = true;
                    else if (m % (k + 1) == 0)      non_isolated = true;
                    if (non_isolated) {
                        size_t cnt = get_count((seq_t)k, idx);
                        value += (seq_t)cnt * (seq_t)k;
                    }
                }
            }
        }
        /* Store the finished term and record it for future lookups. */
        seq_cache[idx] = value;
        add_value(value);
    }
    seq_cache_len = n + 1;   /* all terms up to `n` are now cached.  */
}

/* Public function.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
