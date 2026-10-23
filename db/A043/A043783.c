/*
 * A043783  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 3 mod 5.
 * https://oeis.org/A043783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑3 representation *
 *            has a number of runs congruent to 3 modulo 5. This
 *            implementation * is a simple search: it scans successive
 *            integers until the desired * count is reached. No memoisation is
 *            required, and it works for any * seq_t and idx_t that conform...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   runs_mod5_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/runs_mod5_base3.h"

 

/* OEIS A043783: numbers n such that the number of runs in the base‑3
 * representation of n is congruent to 3 (mod 5). */
seq_t a(idx_t n)
{
    idx_t count = 0;      /* number of qualifying integers found so far */
    idx_t m = 1;          /* candidate integer, start at 1 */

    while (1) {
        if (runs_mod5_base3(m) == 3) {
            count++;
            if (count == n) {
                /* cast to seq_t; works whether seq_t is an integral
                 * type or a C++ class with an appropriate constructor. */
                return (seq_t)m;
            }
        }
        m++;
    }
}
