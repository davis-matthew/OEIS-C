/*
 * A107226  Numbers having in their Zeckendorf representation as many odd as
 *          even terms.
 * https://oeis.org/A107226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose Zeckendorf representation
 *            contains the same number of odd-valued and even-valued Fibonacci
 *            summands
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note:  Zeckendorf representation uses Fibonacci numbers
    F1 = 1, F2 = 2, F3 = 3, F4 = 5, F5 = 8, …   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  static memory for memoisation  */
    static seq_t *found    = NULL;   /* array of found terms   */
    static size_t found_cap = 0;
    static size_t found_len = 0;

    static seq_t *fibs    = NULL;   /* Fibonacci list (>=1,2) */
    static size_t fibs_cap = 0;
    static size_t fibs_len = 0;

    static idx_t last_test = 0;     /* last integer tested */

    /* initialise the Fibonacci list */
    if (fibs_len == 0) {
        fibs_cap = 8;
        fibs = (seq_t *)malloc(fibs_cap * sizeof(seq_t));
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)2;
        fibs_len = 2;
    }

    /* ensure enough space for the result array */
    if (found_len < n) {
        if (found_cap < n) {
            found_cap = n * 2;
            found = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
        }

        /* generate terms until we have at least n of them */
        while (found_len < n) {
            last_test++;                     /* next candidate   */
            seq_t cand = (seq_t) last_test;   /* cast for arithmetic */

            /* count odd/even terms in Zeckendorf representation */
            int even = 0, odd = 0;
            seq_t rem  = cand;

            while (rem > 0) {
                /* extend Fibonacci list if necessary  */
                while (fibs_len > 0 && fibs[fibs_len - 1] < rem) {
                    seq_t next = fibs[fibs_len - 1] + fibs[fibs_len - 2];
                    if (fibs_len == fibs_cap) {
                        fibs_cap = fibs_cap * 2 + 1;
                        fibs = (seq_t *)realloc(fibs, fibs_cap * sizeof(seq_t));
                    }
                    fibs[fibs_len++] = next;
                }

                /* find the largest Fibonacci <= rem */
                size_t i = fibs_len - 1;
                while (fibs[i] > rem) i--;

                seq_t f = fibs[i];

                /* parity test */
                if ((f & (seq_t)1) == 0)
                    even++;
                else
                    odd++;

                rem -= f;
            }

            if (even == odd) {
                found[found_len++] = cand;
            }
        }
    }

    return found[n - 1];
}
