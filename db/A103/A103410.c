/*
 * A103410  Number of products of distinct elements in generation n, starting
 *          with two elements.
 * https://oeis.org/A103410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   choose2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103410 – Number of products of distinct elements in generation n,
   starting with two elements.
   Recurrence:
   a(0) = 2, a(1) = 1
   a(n) = a(n-1) * (a(0)+a(1)+...+a(n-2)) + C(a(n-1),2)   (n ≥ 2)
   where C(x,2) = x*(x-1)/2
   The function memoises results so that calls can arrive in any order. */

#include <stdlib.h>
#include "../utils/choose2.h"

/* Memoisation storage – grows dynamically as needed. */
static seq_t *memo = NULL;     /* a(k) values */
static seq_t *sum_pref = NULL; /* prefix sums Σ a(i), i ≤ k */
static idx_t memo_sz = 0;      /* number of computed terms */

seq_t a(idx_t n)
{
    /* Allocate or grow storage to hold at least n+1 terms. */
    if (n + 1 > memo_sz) {
        idx_t new_sz = n + 1;

        memo     = (seq_t *)realloc(memo,     new_sz * sizeof(seq_t));
        sum_pref = (seq_t *)realloc(sum_pref, new_sz * sizeof(seq_t));

        /* Compute any missing terms up to n. */
        while (memo_sz <= n) {
            if (memo_sz == 0) {
                /* a(0) = 2 */
                memo[0]     = 2;
                sum_pref[0] = 2;
            } else if (memo_sz == 1) {
                /* a(1) = 1 */
                memo[1]     = 1;
                sum_pref[1] = sum_pref[0] + 1;
            } else {
                /* Recurrence for k ≥ 2 */
                seq_t prev   = memo[memo_sz - 1];
                seq_t sum_to = sum_pref[memo_sz - 2];  /* Σ a(0..k-2) */
                seq_t term1  = prev * sum_to;
                seq_t term2  = choose2(prev);
                seq_t cur    = term1 + term2;
                memo[memo_sz]     = cur;
                sum_pref[memo_sz] = sum_pref[memo_sz - 1] + cur;
            }
            memo_sz++;
        }
    }

    return memo[n];
}
