/*
 * A229186  Beginning position of n in the decimal expansion of the
 *          Champernowne constant.
 * https://oeis.org/A229186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the position of the first occurrence of the decimal
   representation of n in Champernowne constant.
   It searches digit‑by‑digit until the target string is matched. */

#include <string.h>   /* for memmove */

seq_t a(idx_t n)
{
    /* convert n to string of decimal digits */
    int len = 1;
    idx_t tmp = n;
    while (tmp >= 10) { tmp /= 10; len++; }

    char target[20];
    tmp = n;
    if (tmp == 0) {
        target[0] = '0';
    } else {
        int pos = len - 1;
        while (tmp > 0) {
            target[pos--] = (char)('0' + (tmp % 10));
            tmp /= 10;
        }
    }

    /* sliding window of the last |target| digits */
    char window[20];
    int wcnt = 0;

    /* current position in the infinite digit sequence (1‑based) */
    idx_t pos = 0;

    /* generate Champernowne digits starting from 1 */
    for (idx_t num = 1; ; num++) {
        /* extract digits of num in reversed order */
        idx_t t = num;
        int digits[20];
        int dcnt = 0;
        while (t > 0) {
            digits[dcnt++] = t % 10;
            t /= 10;
        }
        /* output digits from most significant to least */
        for (int d = dcnt - 1; d >= 0; d--) {
            char ch = (char)('0' + digits[d]);

            /* update sliding window */
            if (wcnt < len) {
                window[wcnt++] = ch;
            } else {
                memmove(window, window + 1, len - 1);
                window[len - 1] = ch;
            }

            pos++;

            /* check for match */
            if (wcnt == len) {
                int k;
                for (k = 0; k < len; k++)
                    if (window[k] != target[k]) break;
                if (k == len)
                    return (seq_t)(pos - (idx_t)len + 1);
            }
        }
    }
}
