/*
 * A043816  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 1 mod 10.
 * https://oeis.org/A043816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   run_count_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043816 – Numbers n whose base‑3 representation has a number of runs ≡ 1 (mod 10)
   This implementation searches for the n‑th such number by scanning integers
   starting at 1 and counting base‑3 runs.  The count is performed on `idx_t`
   values; the resulting nth term is converted to `seq_t` before being returned. */

#include <stdint.h>
#include "../utils/run_count_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; guard against zero index */
        return (seq_t)0;

    idx_t found = 0;      /* how many qualifying numbers we have seen */
    idx_t candidate = 1;  /* current integer to test */

    while (found < n) {
        if (run_count_idx(candidate) % 10 == 1) {
            ++found;
            if (found == n)
                return (seq_t)candidate;   /* cast to seq_t works for all builds */
        }
        ++candidate;
    }

    /* The loop always returns, but this unreachable return satisfies the compiler */
    return (seq_t)0;
}
