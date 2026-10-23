/*
 * A033902  Sort then Add, a(1)=25.
 * https://oeis.org/A033902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=25; for n>1, a(n)=a(n-1)+sorted_ascending_digits(a(n-1))
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   sorted_ascending_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence uses ascending decimal digits, ignoring leading zeros. */
#include <stdbool.h>
#include "../utils/sorted_ascending_digits.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;        /* undefined for offset 1 */
    }

    seq_t cur = (seq_t)25;     /* a(1) */
    for (idx_t i = (idx_t)1; i < n; ++i) {
        cur += sorted_ascending_digits(cur);
    }
    return cur;
}
