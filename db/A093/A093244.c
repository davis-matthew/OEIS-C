/*
 * A093244  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 44.
 * https://oeis.org/A093244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0, a(2)=88, a(3)=70440. * For n≥4: * if n even: a(n) =
 *            8^(n-2) 44 8^(n-2) 4 (concatenation of digits) * if n odd : a(n)
 *            = 7^(n-3) 000 4^(n-3) 000 * This construction yields the longest
 *            number whose all n‑length substrings * are distinct and
 *            divisible by 44
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* special small cases */
    if (n == 1) return seq_from_str("0");
    if (n == 2) return seq_from_str("88");
    if (n == 3) return seq_from_str("70440");

    /* buffer large enough for n up to ~10000 (length ≤ 20000) */
    static char buf[20005];
    size_t len = 0;

    if ((n & 1) == 0) {   /* even n */
        idx_t m = n - 2;          /* number of 8's before and after "44" */
        for (idx_t i = 0; i < m; ++i) buf[len++] = '8';
        buf[len++] = '4';
        buf[len++] = '4';
        for (idx_t i = 0; i < m; ++i) buf[len++] = '8';
        buf[len++] = '4';
    } else {              /* odd n (n≥5) */
        idx_t m = n - 3;          /* number of 7's and 4's */
        for (idx_t i = 0; i < m; ++i) buf[len++] = '7';
        buf[len++] = '0';
        buf[len++] = '0';
        buf[len++] = '0';
        for (idx_t i = 0; i < m; ++i) buf[len++] = '4';
        buf[len++] = '0';
        buf[len++] = '0';
        buf[len++] = '0';
    }
    buf[len] = '\0';
    return seq_from_str(buf);
}
