/*
 * A139142  This sequence and A139143 are complements. a(1) = 1, A139143(1) =
 *          2, a(n+1) = a(n) + Sum_{k = 1..n} A139143(k).
 * https://oeis.org/A139142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 69, u32 n <= 2939, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 1
 *         a(n+1) = a(n) + sum_{k=1..n} b(k)
 * where b(k) is the k-th positive integer not occurring in A(n) (the complement of A).
 * The sequence a(n) is defined by this recurrence and by the complement property.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq_array.h"   /* declares: void grow_seq_array(seq_t **arr, size_t *cap, size_t new_len); */

static seq_t *a_vals   = NULL;   /* array of already computed a(i) values   */
static size_t   a_len   = 0;     /* number of stored a(i) values            */
static size_t   a_cap   = 0;     /* current capacity of a_vals              */
static seq_t   sum_b      = 0;   /* running sum of b(1)..b(k)              */
static seq_t   next_missing = 0; /* next candidate for b(k)                */

/* binary search to test whether val is already an element of a_vals */
static bool contains_a(seq_t val)
{
    size_t lo = 0, hi = a_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a_vals[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < a_len && a_vals[lo] == val);
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; undefined behaviour for n < 1 */
        return (seq_t)0;

    /* initialisation for the first call */
    if (a_len == 0) {
        grow_seq_array(&a_vals, &a_cap, 1);
        a_vals[0] = (seq_t)1;   /* a(1) = 1 */
        a_len = 1;
        sum_b = (seq_t)0;
        next_missing = (seq_t)2; /* first missing integer after 1 */
    }

    /* generate values until we reach the requested index */
    while (a_len < n) {
        /* determine b(k) for k = a_len (since a_len a-values already computed) */
        seq_t b_val = next_missing;
        while (contains_a(b_val))          /* skip any integer already in A */
            b_val = b_val + (seq_t)1;
        next_missing = b_val + (seq_t)1;    /* prepare candidate for next iteration */

        sum_b = sum_b + b_val;              /* update running sum of b's */

        /* compute next a-value */
        seq_t prev_a = a_vals[a_len - 1];
        seq_t next_a = prev_a + sum_b;

        /* enlarge storage if necessary and store the new a-value */
        grow_seq_array(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len] = next_a;
        a_len++;
    }

    return a_vals[n - 1];
}
