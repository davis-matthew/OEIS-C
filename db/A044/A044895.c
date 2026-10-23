/*
 * A044895  Base 9 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_alternating_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044895: Base‑9 run lengths alternate even, odd, even, …
 * A number n is in the sequence if in its base‑9 representation the
 * lengths of consecutive runs of equal digits alternate between even
 * and odd, beginning with a run of even length.
 * The sequence is indexed starting at 1 (a(1)=10).
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_alternating_runs.h"

seq_t a(idx_t n)
{
    /* The sequence starts with a(1) = 10.  Scan consecutive integers
       until the n‑th valid one is found. */
    seq_t value = 10;          /* first term */
    idx_t count = 1;           /* count of terms found so far */

    if (n == 1)
        return value;

    while (1) {
        value++;
        if (has_alternating_runs(value)) {
            count++;
            if (count == n)
                return value;
        }
    }
}
