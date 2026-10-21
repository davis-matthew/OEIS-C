/*
 * A061584  a(1) = 1, a(n)= number obtained by replacing each digit of a(n-1)
 *          with six times its value.
 * https://oeis.org/A061584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   For n > 1, a(n) is obtained from a(n‑1) by replacing each decimal
   digit d with the decimal representation of 6*d, keeping the order.
   Example: a(1)=1 → a(2)=6 → a(3)=36 → a(4)=1836 … */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n <= 1 we return the first term */
    if (n <= 1)
        return (seq_t)1;

    /* Mapping of digits 0..9 to the string of 6*d */
    const char *map[10] = {
        "0", "6", "12", "18", "24", "30",
        "36", "42", "48", "54"
    };
    const size_t maplen[10] = {
        1, 1, 2, 2, 2, 2, 2, 2, 2, 2
    };

    /* Current representation as string, null terminated */
    char *cur = NULL;
    size_t curlen = 0;
    size_t curcap = 0;

    /* Start with "1" */
    curcap = 2;
    cur = (char *)malloc(curcap);
    cur[0] = '1';
    cur[1] = '\0';
    curlen = 1;

    /* Iterate from 2 to n */
    for (idx_t step = 2; step <= n; ++step) {
        /* Determine length of next string */
        size_t newlen = 0;
        for (size_t i = 0; i < curlen; ++i) {
            int d = cur[i] - '0';
            newlen += maplen[d];
        }

        /* Allocate new buffer */
        char *next = (char *)malloc(newlen + 1);  /* +1 for '\0' */
        size_t pos = 0;

        /* Expand each digit */
        for (size_t i = 0; i < curlen; ++i) {
            int d = cur[i] - '0';
            const char *m = map[d];
            size_t ml = maplen[d];
            for (size_t j = 0; j < ml; ++j)
                next[pos++] = m[j];
        }
        next[pos] = '\0';

        /* Free old buffer and swap */
        free(cur);
        cur = next;
        curlen = newlen;
    }

    /* Convert final string to seq_t */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < curlen; ++i) {
        result = result * (seq_t)10 + (seq_t)(cur[i] - '0');
    }

    free(cur);
    return result;
}
