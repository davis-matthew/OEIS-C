/*
 * A031946  Numbers whose base-5 expansions have 5 distinct digits.
 * https://oeis.org/A031946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   all_base5_five_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose base‑5 expansion contains
   all digits 0,1,2,3,4. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/all_base5_five_digits.h"

/* The requested sequence a(n) for n ≥ 1.  Calls are independent and
   the function runs in O(n) time, sufficient for n up to ~10000. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1 */

    seq_t candidate = (seq_t)0;
    idx_t count = 0;
    while (count < n) {
        candidate++;
        if (all_base5_five_digits(candidate)) {
            count++;
        }
    }
    return candidate;
}
