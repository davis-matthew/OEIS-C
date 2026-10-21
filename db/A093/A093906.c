/*
 * A093906  Start with {2} and close under the operations XY and XY+1;
 *          sequence gives complete list of numbers that do not appear.
 * https://oeis.org/A093906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th positive integer that cannot be expressed as X*Y or
 *            X*Y+1 for any X,Y in the closure of {2} under the operations XY
 *            and XY+1
 * Range:     u8 n <= 52, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static idx_t current_limit = 0;
    static idx_t missing_len   = 0;
    static seq_t *missing_arr  = NULL;
    static size_t missing_cap  = 0;

    if (n == 1) /* a(1) = 1 */
        return (seq_t)1;

    /* Ensure we have computed enough missing values. */
    if (missing_len < n) {
        /* Determine an initial limit large enough for the required n. */
        idx_t target = n * 3 + 20;      /* heuristic upper bound */
        if (current_limit == 0)
            current_limit = 4;          /* start with a small limit */

        if (current_limit < target)
            current_limit = target;

        /* Recompute until we have at least n missing numbers. */
        for (;;) {
            idx_t limit = current_limit;

            /* Allocate and clear the array of reachable numbers. */
            bool *reachable = (bool *)calloc((size_t)limit + 1, sizeof(bool));
            if (!reachable) /* allocation failure */
                return (seq_t)0;
            reachable[2] = true; /* start set contains only 2 */

            /* Allocate (or reallocate) the missing values array. */
            size_t new_cap = (size_t)limit + 2;   /* guarantee enough space */
            seq_t *new_arr = (seq_t *)realloc(missing_arr,
                                               new_cap * sizeof(seq_t));
            if (!new_arr) {
                free(reachable);
                return (seq_t)0;
            }
            missing_arr = new_arr;
            missing_cap = new_cap;
            missing_len = 0;

            /* The very first missing number is 1. */
            missing_arr[missing_len++] = (seq_t)1;

            /* Build the closure and collect missing numbers up to 'limit'. */
            for (idx_t i = 3; i <= limit; ++i) {
                bool found = false;

                /* Check for the product case: i = X * Y. */
                for (idx_t d = 2; d * d <= i; ++d) {
                    if (i % d == 0) {
                        idx_t e = i / d;
                        if (reachable[d] && reachable[e]) {
                            found = true;
                            break;
                        }
                    }
                }

                /* If not found, check the +1 case: i = X * Y + 1. */
                if (!found) {
                    idx_t m = i - 1;
                    for (idx_t d = 2; d * d <= m; ++d) {
                        if (m % d == 0) {
                            idx_t e = m / d;
                            if (reachable[d] && reachable[e]) {
                                found = true;
                                break;
                            }
                        }
                    }
                }

                if (found) {
                    reachable[i] = true;          /* i is reachable */
                } else {
                    if (missing_len < missing_cap)
                        missing_arr[missing_len++] = (seq_t)i; /* store missing */
                }
            }

            free(reachable);

            if (missing_len >= n)
                break;   /* sufficient data obtained */

            /* Not enough missing numbers – double the limit and try again. */
            current_limit = limit * 2;
        }
    }

    return missing_arr[n - 1];
}
