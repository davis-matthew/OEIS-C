/*
 * A043705  Numbers whose base-16 representation has an even number of runs.
 * https://oeis.org/A043705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_hex_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043705: Numbers whose base‑16 representation has an even number of runs
   (enumerated in increasing order). This implementation scans integers
   and counts the runs in their hexadecimal representation.  */
#include <stdbool.h>
#include "../utils/even_hex_runs.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;        /* start inspecting from 1 */

    while (1) {
        if (even_hex_runs(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;   /* cast to return type */
        }
        candidate++;
    }
}
