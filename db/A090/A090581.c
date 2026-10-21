/*
 * A090581  Number of numbers with 6 decimal digits and sum of digits = n.
 * https://oeis.org/A090581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = number of 6‑digit integers (from 100000 to 999999) whose
 *            decimal digits sum to n. a(1) corresponds to sum = 1. For n < 1
 *            or n > 54 the value is defined to be 0
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM 54      /* largest possible digit sum: 9×6 */

seq_t a(idx_t n)
{
    /* the count for every possible sum 0 … MAX_SUM is stored in a static
       array.  The array is populated only once, on the first call.   */
    static seq_t counts[MAX_SUM + 1];
    static bool initialized = false;

    if (!initialized) {
        /* initialise all entries to zero */
        for (int i = 0; i <= MAX_SUM; ++i)
            counts[i] = 0;

        /* enumerate all six‑digit numbers (100000 – 999999) and count the
           occurrences of each digit sum.  The enumeration is inexpensive
           (900 000 iterations).  The loops are typed with idx_t to avoid
           signed/unsigned mismatches. */
        for (idx_t d1 = 1; d1 <= 9; ++d1) {          /* leading digit cannot be 0 */
            for (idx_t d2 = 0; d2 <= 9; ++d2) {
                for (idx_t d3 = 0; d3 <= 9; ++d3) {
                    for (idx_t d4 = 0; d4 <= 9; ++d4) {
                        for (idx_t d5 = 0; d5 <= 9; ++d5) {
                            for (idx_t d6 = 0; d6 <= 9; ++d6) {
                                idx_t sum = d1 + d2 + d3 + d4 + d5 + d6;
                                /* sum ∈ [1,54] so an int cast is safe */
                                counts[(int)sum] += (seq_t)1;
                            }
                        }
                    }
                }
            }
        }
        initialized = true;
    }

    if (n >= 1 && n <= (idx_t)MAX_SUM)
        return counts[(int)n];
    else
        return 0;
}
