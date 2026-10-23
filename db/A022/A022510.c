/*
 * A022510  Describe previous term from the right (method A - initial term is
 *          6).
 * https://oeis.org/A022510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0) = 6; for n>0, a(n) is obtained by applying the “describe
 *            previous term from the right” rule repeatedly. No hard‑coded
 *            table is used
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)6;

    /* Start with a(0) = "6".  Allocate two bytes: one for '6' and one for '\0'. */
    char *curr = (char *)malloc(2);
    curr[0] = '6';
    curr[1] = '\0';

    for (idx_t i = 1; i <= n; ++i) {
        size_t len = strlen(curr);
        /* Worst case: each digit becomes a group of size 1, producing
           two characters per original digit.  Add space for '\0'. */
        size_t out_cap = 2 * len + 1;
        char *out = (char *)malloc(out_cap);
        size_t out_idx = 0;
        size_t pos = len;

        while (pos > 0) {
            char digit = curr[pos - 1];
            size_t count = 1;
            pos--;
            while (pos > 0 && curr[pos - 1] == digit) {
                count++;
                pos--;
            }
            /* Convert count to decimal.  Count is always >= 1. */
            char cntbuf[32];
            int cntlen = 0;
            size_t tmp = count;
            do {
                cntbuf[cntlen++] = (char)('0' + (tmp % 10));
                tmp /= 10;
            } while (tmp > 0);
            for (int k = cntlen - 1; k >= 0; --k)
                out[out_idx++] = cntbuf[k];
            out[out_idx++] = digit;
        }

        out[out_idx] = '\0';

        free(curr);
        curr = out;
    }

    seq_t result = seq_from_str(curr);
    free(curr);
    return result;
}
