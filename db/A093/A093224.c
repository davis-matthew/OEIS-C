/*
 * A093224  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 24.
 * https://oeis.org/A093224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the largest integer whose length‑n substrings are
 *            distinct and all divisible by 24. For n=1…3 the values are
 *            special cases; for n≥4 the digits follow a simple constructive
 *            pattern: a(n) = (n-3)×'9' + "55" + "2" + (n-3)×'0' + "888" +
 *            (n-3)×'0' + "88" The result is returned as a seq_t conve...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)7248;
    if (n == 3)
        return (seq_t)936000;

    /* For n >= 4, build the decimal representation described above. */
    size_t buf_len = 3 * (size_t)n;     /* 3n digits plus NUL */
    char *buf = (char *)malloc(buf_len);
    if (!buf)
        exit(1);                       /* allocation failure */

    size_t pos = 0;
    /* (n-3) '9's */
    for (idx_t i = 0; i < n - 3; ++i)
        buf[pos++] = '9';

    /* '55' */
    buf[pos++] = '5';
    buf[pos++] = '5';
    /* '2' */
    buf[pos++] = '2';

    /* (n-3) '0's */
    for (idx_t i = 0; i < n - 3; ++i)
        buf[pos++] = '0';

    /* '888' */
    buf[pos++] = '8';
    buf[pos++] = '8';
    buf[pos++] = '8';

    /* (n-3) '0's again */
    for (idx_t i = 0; i < n - 3; ++i)
        buf[pos++] = '0';

    /* '88' */
    buf[pos++] = '8';
    buf[pos++] = '8';

    buf[pos] = '\0';  /* null termination */

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
