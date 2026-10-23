/*
 * A037993  Numbers whose maximal base-8 run length is 2.
 * https://oeis.org/A037993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_valid_oct_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037993: Numbers whose maximal base-8 run length is 2 (search) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_valid_oct_run.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behavior below offset is undefined */
    idx_t count = 0;
    seq_t cand = 1;                     /* start from 1 (0 never satisfies) */

    for (;; ++cand) {
        if (has_valid_oct_run(cand)) {
            count++;
            if (count == n) return cand;
        }
    }
}
