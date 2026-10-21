/*
 * A093215  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 15.
 * https://oeis.org/A093215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) for n ≥ 4 is built as [n-3 times '9'] + "88" + "5" + [n-3
 *            times '0'] + "555" + [n-3 times '0'] + "5" (the first three
 *            terms are defined separately)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* offset 1 */
    if (n == 1)
        return seq_from_str("0");
    if (n == 2)
        return seq_from_str("900");
    if (n == 3)
        return seq_from_str("99000");

    /* n >= 4 */
    size_t l = (size_t)(n - 3);           /* number of leading 9s and zero blocks */
    size_t len = 3 * l + 7;              /* total number of digits in the result */
    char *s = (char *)malloc(len + 1);
    if (!s)                               /* out of memory – return 0 */
        return seq_from_str("0");

    size_t pos = 0;
    /* leading 9's */
    for (size_t i = 0; i < l; ++i) s[pos++] = '9';

    /* "88" */
    s[pos++] = '8';
    s[pos++] = '8';

    /* "5" */
    s[pos++] = '5';

    /* zeros of length l */
    for (size_t i = 0; i < l; ++i) s[pos++] = '0';

    /* "555" */
    s[pos++] = '5';
    s[pos++] = '5';
    s[pos++] = '5';

    /* zeros of length l */
    for (size_t i = 0; i < l; ++i) s[pos++] = '0';

    /* final "5" */
    s[pos++] = '5';
    s[pos] = '\0';

    seq_t result = seq_from_str(s);
    free(s);
    return result;
}
