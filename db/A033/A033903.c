/*
 * A033903  Sort then Add, a(1)=27.
 * https://oeis.org/A033903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 50, u64 n <= 356,
 *            u128 n <= 1000, big any n
 * Helpers:   sort_ascending_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 27,
    a(n) = a(n-1) + sort_ascending_digits(a(n-1))   for n ≥ 2
   where sort_ascending_digits(x) forms the integer obtained by sorting the
   decimal digits of x in increasing order (leading zeros are dropped). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sort_ascending_digits.h"

seq_t a(idx_t n)
{
    /* n is 1‑indexed; a(1) = 27 */
    seq_t val = 27;

    /* Compute a(n) iteratively; O(n) time, negligible for n ≤ 10 000 */
    for (idx_t i = 1; i < n; ++i)
        val += sort_ascending_digits(val);

    return val;
}
