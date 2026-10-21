/*
 * A228014  Numbers k whose Collatz sequence length is greater than k+1
 *          (counting both the x/2 and the 3x+1 steps).
 * https://oeis.org/A228014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228014: numbers k whose Collatz sequence length (in terms)
   is greater than k+1. This implementation keeps a static list of
   found numbers and extends it until it contains the requested terms. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *vals      = NULL;   /* array of found k's               */
    static size_t vals_len  = 0;       /* number of values currently stored */
    static size_t vals_cap  = 0;       /* current allocation capacity      */
    static idx_t  next_k    = 1;       /* next integer to test for Collatz  */

    /* If n is zero or negative we return 0 (undefined behaviour otherwise) */
    if (n == 0) {
        return 0;
    }

    /* Grow the list until it contains at least n terms */
    while (vals_len < n) {
        seq_t x     = (seq_t)next_k;   /* working copy in the Collatz loop   */
        size_t steps = 0;                /* number of transitions to 1         */

        /* Compute total stopping time for next_k (in transitions) */
        while (x != (seq_t)1) {
            if ((x & (seq_t)1) == 0) {      /* even */
                x = x >> 1;
            } else {                        /* odd */
                x = x * 3 + 1;
            }
            ++steps;
        }

        /* Sequence length in terms is steps + 1; require that > k+1
           which is equivalent to steps > k.                            */
        if (steps > (size_t)next_k) {
            /* Grow the array if needed */
            if (vals_len == vals_cap) {
                size_t new_cap = vals_cap ? vals_cap << 1 : 1;
                seq_t *new_vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                if (!new_vals) {            /* allocation failed */
                    /* In a real program we would handle the error. */
                    return 0;
                }
                vals      = new_vals;
                vals_cap  = new_cap;
            }
            vals[vals_len] = (seq_t)next_k;
            ++vals_len;
        }
        ++next_k;
    }

    /* Return the (n‑1)-th element of the list (0‑based indexing) */
    return vals[n - 1];
}
