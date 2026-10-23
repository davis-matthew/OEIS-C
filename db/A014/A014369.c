/*
 * A014369  a(n) = bcd, where n = C(b,3)+C(c,2)+C(d,1), b>c>d>=0.
 * https://oeis.org/A014369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = concatenation of b,c,d where
        n = C(b,3)+C(c,2)+d with b>c>d≥0.
   Sequence is indexed with a(1)=310 etc. */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* a(1) is the first defined term; below offset behaviour is irrelevant */
    if (n < 1) return 0;

    /* Find b such that the cumulative number of triples up to b-1
       is < n ≤ number of triples up to b.  The number of triples for
       a given b is (b-1)*b/2. */
    idx_t sum = 0;
    idx_t b = 3;                       /* minimal b that yields triples */
    for (;;) {
        idx_t block = (b - 1) * b / 2; /* triples with this b */
        if (sum + block >= n) break;
        sum += block;
        ++b;
    }

    /* Offset inside the current block (0‑based). */
    idx_t offset = n - sum - 1;

    /* Determine c: the smallest c such that offset < c.  The number
       of triples with a given c is exactly c (since d runs from 0 to c-1). */
    idx_t c = 1;
    while (offset >= c) {
        offset -= c;
        ++c;
    }

    /* Remaining offset is d. */
    idx_t d = offset;

    /* Concatenate b, c and d as decimal strings.  Use pow10 to
       shift the current value left by the number of digits of the
       next component.  digits(0) returns 1, so no special case needed. */
    seq_t result = b;
    result = result * pow10((idx_t)digits(c)) + c;
    result = result * pow10((idx_t)digits(d)) + d;

    return result;
}
