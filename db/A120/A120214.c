/*
 * A120214  Start with 1013 and repeatedly reverse the digits and add 2 to get
 *          the next term.
 * https://oeis.org/A120214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1013
   a(n) = reverse_decimal(a(n-1)) + 2  for n > 1
   The sequence enters a cycle of length 90, which is precomputed once
   and afterwards accessed by indexing (n-1) mod 90.
*/

#include <stdbool.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    static seq_t cycle[90];
    static bool initialized = false;

    if (!initialized) {
        seq_t cur = (seq_t)1013;          /* a(1) */
        for (idx_t i = 0; i < 90; ++i) {
            cycle[i] = cur;
            cur = reverse_decimal(cur) + (seq_t)2;
        }
        initialized = true;
    }

    /* n >= 1; return (n-1) modulo 90 */
    idx_t idx = (n - 1) % 90;
    return cycle[idx];
}
