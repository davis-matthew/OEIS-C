/*
 * A146124  Number of n X n arrays of squares of integers summing to 5 with
 *          every element equal to at least one neighbor.
 * https://oeis.org/A146124
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
 * for n > 8, with the following initial values (a(2) is the offset):
 *   a(2) = 0
 *   a(3) = 32
 *   a(4) = 516
 *   a(5) = 2307
 *   a(6) = 6472
 *   a(7) = 14379
 *   a(8) = 27684
 */

#include <stdbool.h>   /* for bool */

seq_t a(idx_t n)
{
    /* Handle the offset and values below it (undefined behaviour
       is not critical, we simply return 0). */
    if (n < 2)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)0;   /* a(2) */
    if (n == 3)
        return (seq_t)32;  /* a(3) */
    if (n == 4)
        return (seq_t)516;  /* a(4) */
    if (n == 5)
        return (seq_t)2307; /* a(5) */
    if (n == 6)
        return (seq_t)6472; /* a(6) */
    if (n == 7)
        return (seq_t)14379;/* a(7) */
    if (n == 8)
        return (seq_t)27684;/* a(8) */

    /* For n >= 9 use the linear recurrence with a sliding window
       of the last five values. This is O(n) and needs only
       constant extra space. */
    seq_t window[5];
    window[0] = (seq_t)516;   /* a(4)  */
    window[1] = (seq_t)2307;  /* a(5)  */
    window[2] = (seq_t)6472;  /* a(6)  */
    window[3] = (seq_t)14379; /* a(7)  */
    window[4] = (seq_t)27684; /* a(8)  */

    seq_t term = 0;
    for (idx_t i = 9; i <= n; ++i) {
        term = 5 * window[4]          /* 5*a(n-1) */
             - 10 * window[3]         /* -10*a(n-2) */
             + 10 * window[2]         /* +10*a(n-3) */
             - 5 * window[1]          /* -5*a(n-4) */
             + window[0];             /* +a(n-5) */

        /* Slide the window: discard old, shift, and push new. */
        window[0] = window[1];
        window[1] = window[2];
        window[2] = window[3];
        window[3] = window[4];
        window[4] = term;
    }

    return term;
}
