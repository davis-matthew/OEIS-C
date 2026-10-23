/*
 * A145470  A positive integer k is included if d(k) = d(k+1) + d(k+2), where
 *          d(k) is the number of divisors of k.
 * https://oeis.org/A145470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that d(k)=d(k+1)+d(k+2), where
 *            d(m) is the number of divisors of m
 * Range:     u8 n <= 8, u16 n <= 2783, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation incrementally searches the integers k
   beginning at 1, computing divisor counts on the fly.  The found
   sequence values are cached in a static array so that calls with
   arbitrary ordering of n are efficient.  The helper function
   divisor_count_u64 is used to compute d(k). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

/* Optional helper to grow the dynamic array */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need) {
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : *cap * 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* External function required by the build */
seq_t a(idx_t n) {
    /* static state */
    static seq_t *found = NULL;
    static size_t found_cap = 0;
    static size_t found_len = 0;
    static uint64_t last_checked_k = 0;

    /* Return cached value if already computed */
    if (n <= found_len) return found[n - 1];

    /* Continue searching until we have found n terms */
    for (;;) {
        last_checked_k++;
        uint64_t k = last_checked_k;

        seq_t d1 = divisor_count_u64(k);
        seq_t d2 = divisor_count_u64(k + 1);
        seq_t d3 = divisor_count_u64(k + 2);

        if (d1 == d2 + d3) {
            if (found_len == found_cap)
                grow_seq_array(&found, &found_cap, found_len + 1);
            found[found_len++] = k;
            if (found_len == n) break;
        }
    }

    return found[n - 1];
}
