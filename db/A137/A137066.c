/*
 * A137066  Numbers k such that k and k^2 use only the digits 2, 3, 4 and 5.
 * https://oeis.org/A137066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation generates the OEIS sequence A137066, defined as
   all positive integers k composed only of digits 2,3,4,5 such that
   k^2 also consists exclusively of those digits.
   The sequence consists of an infinite family of pattern numbers
   p_m = 2*10^m + (10^m-1)/3 + 2   (for m ≥ 2) and a finite set of
   exception numbers that do not belong to this family.
   The pattern numbers satisfy the simple recurrence
       p_{m+1} = 10*p_m - 15.
   The exception numbers are: 2, 5, 2354235, 2333524235, 23333524235.
   The sequence is obtained by merging these two sorted lists.
*/

#include <stddef.h>   /* for size_t */

#ifndef NO_IMPLEMENTATION
seq_t a(idx_t n) {
    /* Exception numbers in ascending order */
    static const seq_t exceptions[] = {
        (seq_t)2,
        (seq_t)5,
        (seq_t)2354235,
        (seq_t)2333524235,
        (seq_t)23333524235
    };
    const size_t n_ex = sizeof(exceptions) / sizeof(exceptions[0]);

    /* Initial pattern number (m = 2). */
    seq_t pat = (seq_t)235;      /* 2*10^2 + (10^2-1)/3 + 2 */
    size_t exc_idx = 0;          /* index of next exception to output */
    idx_t count = 0;             /* number of terms produced so far */

    while (1) {
        seq_t next;
        int use_pattern = 0;

        if (exc_idx < n_ex) {
            if (pat < exceptions[exc_idx]) {
                next = pat;
                use_pattern = 1;
            } else {
                next = exceptions[exc_idx];
            }
        } else {
            next = pat;
            use_pattern = 1;
        }

        ++count;
        if (count == n)
            return next;

        /* Advance to next term in the chosen subsequence */
        if (use_pattern) {
            /* Generate the next pattern number using the recurrence. */
            pat = (seq_t)(10 * pat - 15);
        } else {
            ++exc_idx;
        }
    }
}
#endif
