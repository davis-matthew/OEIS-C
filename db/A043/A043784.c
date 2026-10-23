/*
 * A043784  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 4 mod 5.
 * https://oeis.org/A043784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   count_runs_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_runs_base3.h"
/* Numbers n for which the number of runs in the base‑3 representation
 * of n is congruent to 4 (mod 5).  Computed by incremental search. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n==0 – return 0 just in case */

    idx_t count = 0;           /* how many qualifying numbers seen */
    idx_t candidate = 1;       /* next integer to test */

    while (1) {
        if (count_runs_base3(candidate) % 5 == 4) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
