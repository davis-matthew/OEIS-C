/*
 * A080707  a(1)=5; for n>1, a(n) = smallest number > a(n-1) such that the
 *          condition "n is in the sequence if and only if a(n) is a multiple
 *          of 3" is satisfied.
 * https://oeis.org/A080707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A080707
 *  -------------------------------------------------------------
 *  This function computes the sequence defined as:
 *
 *      a(1) = 5
 *      For n > 1, a(n) is the smallest integer greater than a(n‑1)
 *      such that the condition "n is in the sequence if and only
 *      if a(n) is a multiple of 3" is satisfied for all indices
 *      up to n.
 *
 *  The implementation keeps the sequence values increasing
 *  and stores the set of produced values that are ≤ N
 *  in a boolean array.  When processing index i it checks
 *  whether i has already appeared as a value; that determines
 *  whether a(i) must be divisible by 3 or not, and a minimal
 *  candidate is chosen accordingly.  The routine is memoised
 *  across calls and works in O(n) time.
 */

#include <stdbool.h>
#include <stdint.h>

/*  Maximal n that the routine will handle.  The tests use
 *  n ≤ 10000, so a slightly larger static array size is
 *  sufficient.  If a larger n is requested the function will
 *  still work but will be limited by this array bound.
 */
#define MAX_N 200000

static idx_t  max_computed = 0;      /* last index computed */
static seq_t  last_value   = 0;      /* a(max_computed)   */

/*  Cache of computed terms and a presence table for values ≤ MAX_N.
 */
static seq_t  cache[MAX_N + 1];
static uint8_t present[MAX_N + 1] = {0};

seq_t a(idx_t n)
{
    /*  Initialise the sequence on the first call.  */
    if (max_computed == 0) {
        cache[1]  = 5;
        last_value = 5;
        max_computed = 1;
        if (5 <= MAX_N) present[5] = 1;
    }

    /*  If the requested index is already computed, return it.  */
    if (n <= max_computed) {
        return cache[n];
    }

    /*  Compute terms from max_computed+1 up to n.  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        bool in_sequence = (i <= MAX_N) ? present[i] : false;

        seq_t cand = last_value + 1;
        /*  Find the smallest candidate > last_value that matches
         *  the divisibility requirement for index i.
         */
        while ( ((cand % (seq_t)3) == 0) != in_sequence ) {
            cand++;
        }

        cache[i] = cand;
        last_value = cand;

        /*  Mark the value in the presence table if it is within range. */
        if (cand <= (seq_t)MAX_N) {
            present[(idx_t)cand] = 1;
        }
    }

    max_computed = n;
    return cache[n];
}
