/*
 * A037989  Numbers whose maximal base-7 run length is 2.
 * https://oeis.org/A037989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_max_run_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose maximal base‑7 run length is exactly 2.
   The function scans successive integers and returns the n‑th one that
   satisfies the run‑length condition. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_max_run_two.h"

/* Return the n‑th term of OEIS sequence A037989.
   n is assumed to be a positive integer (>=1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 8;                    /* first term confirmed to be 8            */

    while (1) {
        if (has_max_run_two(m)) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m++;
    }
}
