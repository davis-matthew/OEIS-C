/*
 * A033898  Sort then Add, a(1)=15.
 * https://oeis.org/A033898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=15; a(n)=a(n-1)+sort_digits_ascending(a(n-1)) for n>1
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 62, u64 n <= 327,
 *            u128 n <= 1000, big any n
 * Helpers:   sort_digits_ascending
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sort_digits_ascending.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)15;
    seq_t val = (seq_t)15;
    for (idx_t i = 2; i <= n; ++i)
        val += sort_digits_ascending(val);
    return val;
}
