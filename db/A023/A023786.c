/*
 * A023786  Katadromes: digits in base 4 are in strict descending order.
 * https://oeis.org/A023786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023786:
   numbers whose base‑4 representation has strictly decreasing digits.
   The terms are obtained by taking all non‑empty subsets of the digits
   {0,1,2,3}, ordering each subset in descending order, and interpreting
   the resulting digit string as a base‑4 number.
*/

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of all terms, built once */
    static seq_t values[16];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* enumerate all subsets of {0,1,2,3} */
        for (idx_t mask = 0; mask < ((idx_t)1 << 4); ++mask) {
            seq_t val = 0;
            /* build the base‑4 number with digits in descending order */
            for (int d = 3; d >= 0; --d) {
                if (mask & ((idx_t)1 << d)) {
                    val = val * ((seq_t)4) + ((seq_t)d);
                }
            }
            /* insert if not already present (duplicate 0 from empty set) */
            bool dup = false;
            for (size_t i = 0; i < count; ++i) {
                if (values[i] == val) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                values[count++] = val;
            }
        }
        /* sort the values ascending */
        for (size_t i = 0; i + 1 < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (values[i] > values[j]) {
                    seq_t t      = values[i];
                    values[i] = values[j];
                    values[j] = t;
                }
            }
        }
        initialized = true;
    }

    if (n >= (idx_t)1 && n <= (idx_t)count) {
        return values[(size_t)(n - 1)];
    }
    /* for indices beyond the finite sequence, return 0 */
    return (seq_t)0;
}
