/*
 * A117952  Perrin numbers which are divisible by the sum of their digits.
 * https://oeis.org/A117952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 14, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   digit_sum, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A117952: Perrin numbers divisible by the sum of their digits, 
   outputting each distinct such number in order of increasing Perrin index.
   Perrin numbers are defined by:
       P(0) = 3, P(1) = 0, P(2) = 2, and P(k) = P(k-2) + P(k-3) for k ≥ 3.
   a(n) is the nth distinct Perrin number P(k) satisfying digit_sum(P(k)) | P(k).
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/grow_seq.h"

static seq_t *matches = NULL;          /* stored matches of the sequence   */
static size_t matches_count = 0;       /* number of matches found so far   */
static size_t matches_cap   = 0;       /* allocated capacity of matches    */
static bool initialized = false;      /* whether static data has been set */

/* data for generating Perrin numbers */
static seq_t p_prev2, p_prev1, p_curr; /* P(k-2), P(k-1), P(k)              */
static idx_t cur_k;                    /* current Perrin index k              */

/* Extend the stored matches until we have at least 'target' terms. */
static void extend_to(idx_t target)
{
    if (matches_count >= (size_t)target)
        return;

    if (!initialized) {
        /* initialise Perrin sequence */
        p_prev2 = 3;          /* P(0) */
        p_prev1 = 0;          /* P(1) */
        p_curr  = 2;          /* P(2) */
        cur_k   = 2;          /* current index */
        initialized = true;

        /* test the first Perrin number (k = 2) */
        seq_t sum = digit_sum(p_curr);
        if (sum != 0 && p_curr % sum == 0) {
            grow_seq(&matches, &matches_cap, matches_count + 1);
            matches[matches_count++] = p_curr;
        }
    }

    while (matches_count < (size_t)target) {
        /* compute next Perrin number: P(k+1) = P(k-1) + P(k-2) */
        seq_t next = p_prev1 + p_prev2;
        /* shift window */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        p_curr  = next;
        cur_k++;

        /* test the new Perrin number */
        seq_t sum = digit_sum(p_curr);
        if (sum != 0 && p_curr % sum == 0) {
            /* avoid duplicate values */
            bool seen = false;
            for (size_t i = 0; i < matches_count; ++i) {
                if (matches[i] == p_curr) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                grow_seq(&matches, &matches_cap, matches_count + 1);
                matches[matches_count++] = p_curr;
            }
        }
    }
}

/* Returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but avoid UB */
    extend_to(n);
    return matches[n - 1];
}
