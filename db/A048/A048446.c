/*
 * A048446  Take the first n numbers written in base 15, concatenate them,
 *          then convert from base 15 to base 10.
 * https://oeis.org/A048446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   digits_base15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) * 15^d(n) + n, a(1) = 1; d(n) = number of base‑15 digits of n. */

#include <stddef.h>
#include "../utils/digits_base15.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        size_t len = digits_base15(i);
        seq_t pow = 1;
        for (size_t j = 0; j < len; ++j)
            pow *= 15;  /* 15 as seq_t */
        result = result * pow + i;    /* i added as seq_t implicitly */
    }
    return result;
}
