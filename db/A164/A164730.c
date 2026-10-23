/*
 * A164730  Least element of each cycle of length 7 under the Kaprekar map
 *          A151949.
 * https://oeis.org/A164730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4 * 10^{2n+3} + (10^{n-1}-1)/3 * 10^{n+4} + 2087 * 10^n +
 *            (10^n-1)/9 i.e. the decimal string consists of 4, (n‑1) copies
 *            of 3, the block 2087, and n copies of 6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 7, u128 n <= 17,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0) return seq_from_str("0");

    /* Length of the decimal string: 1 digit '4' + (n-1) digits '3'
       + 4 digits "2087" + n digits '6'  = 2n + 4 digits. */
    size_t len = 2 * (size_t)n + 4;
    char *buf = (char *)malloc(len + 1);          /* +1 for NUL terminator */
    if (!buf) return seq_from_str("0");           /* fallback if allocation fails */

    char *p = buf;

    /* Leading '4' */
    *p++ = '4';

    /* (n-1) copies of '3' */
    for (idx_t i = 0; i < n - 1; ++i) *p++ = '3';

    /* '2087' */
    *p++ = '2';
    *p++ = '0';
    *p++ = '8';
    *p++ = '7';

    /* n copies of '6' */
    for (idx_t i = 0; i < n; ++i) *p++ = '6';

    *p = '\0';

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
