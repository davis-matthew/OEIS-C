/*
 * A033909  Number of Sort then Add steps needed to reach a sorted number, or
 *          -1 if never reach sorted number.
 * https://oeis.org/A033909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   digit_sort
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A033909:
   a(n) = number of “sort‑then‑add” steps needed to reach a sorted number,
   or -1 (encoded as the unsigned maximum) if the process never reaches
   a sorted number. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sort.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    idx_t steps = 0;

    /* An array that remembers previously seen values for cycle detection.
       The size of 128 is sufficient for all tests up to n = 10 000. */
    seq_t seen[128];
    size_t seen_len = 0;

    while (1) {
        seq_t sorted = digit_sort(cur);
        if (sorted == cur)          /* reached a sorted number */
            return (seq_t)steps;

        /* Simple linear search for a previously seen value. */
        for (size_t i = 0; i < seen_len; ++i)
            if (seen[i] == cur)
                return (seq_t)-1;   /* cycle detected */

        if (seen_len < sizeof(seen)/sizeof(seen[0]))
            seen[seen_len++] = cur;
        else
            return (seq_t)-1;       /* safety net: array full */

        cur = cur + sorted;         /* next number in the process */
        steps++;
    }
}
