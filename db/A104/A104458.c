/*
 * A104458  Define the first two terms to be 2 and 3. All the other terms are
 *          obtained by concatenating the two previous terms.
 * https://oeis.org/A104458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=3; for n>=3, a(n) = concatenation of a(n-2) and a(n-1) */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* handle the two fixed initial terms */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* string buffers for the last two terms */
    char *prev2 = (char*)malloc(2); /* "2" plus '\0' */
    char *prev1 = (char*)malloc(2); /* "3" plus '\0' */
    if (!prev2 || !prev1) return (seq_t)0; /* allocation failed */
    prev2[0] = '2'; prev2[1] = '\0';
    prev1[0] = '3'; prev1[1] = '\0';

    idx_t len2 = 1;
    idx_t len1 = 1;
    char *newbuf = NULL;

    for (idx_t i = 3; i <= n; ++i) {
        idx_t len_new = len2 + len1;
        newbuf = (char*)malloc((size_t)len_new + 1);
        if (!newbuf) break;
        memcpy(newbuf, prev2, (size_t)len2);
        memcpy(newbuf + len2, prev1, (size_t)len1);
        newbuf[len_new] = '\0';

        /* rotate buffers */
        free(prev2);
        prev2 = prev1;
        len2 = len1;
        prev1 = newbuf;
        len1 = len_new;
    }

    /* convert the resulting string to seq_t */
    seq_t value = 0;
    char *p = prev1;
    while (*p) {
        value = value * 10 + (seq_t)(*p - '0');
        ++p;
    }

    free(prev1);
    return value;
}
