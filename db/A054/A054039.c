/*
 * A054039  a(n)^2 is the least square to contain n different decimal digits.
 * https://oeis.org/A054039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   count_distinct_decimal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute a(n): the least integer m such that m^2 contains at least
 * n distinct decimal digits.  a(1) = 0 (the empty square 0³ = 0).
 * The routine performs a linear search over consecutive squares,
 * recording the first m that achieves each new digit count up to 10.
 * Results are memoised so subsequent calls are O(1) for already
 * computed indices.
 */
#include <stdbool.h>
#include "../utils/count_distinct_decimal_digits.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0 or negative */

    /* static state --------------------------------------------------- */
    static bool     init      = false;
    static seq_t    cache[11];   /* indices 1..10, 0 unused for safety   */
    static idx_t    next_m    = 1; /* next base to test (m)               */
    static idx_t    max_found = 1; /* largest n for which we already know the answer  */
    static bool     finished  = false; /* reached n = 10, no larger n possible  */

    if (!init) {                 /* initialise on first call */
        for (int i = 0; i < 11; i++) cache[i] = 0;
        cache[1]   = 0;          /* a(1) = 0 */
        init       = true;
    }

    /* return known value if the index is already satisfied  */
    if (n <= max_found) return cache[n];

    /* continue searching until we reach the requested n or
       the maximal possible digit count (10)  */
    if (!finished) {
        for (;;) {
            ++next_m;                                 /* try next m      */
            seq_t sq = (seq_t)next_m * (seq_t)next_m; /* m² with arbitrary precision  */
            int    d  = count_distinct_decimal_digits(sq);

            if ((idx_t)d > max_found) {               /* new distinct count found */
                if ((idx_t)d <= 10) {                 /* store only up to 10 */
                    cache[d] = (seq_t)next_m;
                    max_found = (idx_t)d;
                }
                if (max_found >= 10) {                /* cannot exceed 10 digits */
                    finished = true;
                    break;
                }
                if (max_found >= n) break;            /* stop if we have the needed index */
            }
            /* otherwise keep searching */
        }
    }

    return (n <= max_found) ? cache[n] : 0; /* return 0 if n > 10 */
}
