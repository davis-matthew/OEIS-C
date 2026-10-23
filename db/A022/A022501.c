/*
 * A022501  Describe the previous term! (method B - initial term is 5).
 * https://oeis.org/A022501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5; for n>1, a(n)=RunLen(a(n-1)) */
/* RunLen(x) outputs each maximal block of identical digits in x as
 * "<digit><length>" (decimal).  The blocks are kept in order.  */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* initial term */
    char *curr = (char*)malloc(2);          /* "5" + '\0' */
    curr[0] = '5';
    curr[1] = '\0';
    size_t curr_len = 1;

    /* generate terms up to n (0-indexed inside the loop) */
    for (idx_t i = 2; i <= n; ++i) {
        /* estimate next string length */
        size_t next_len_est = curr_len * 2 + 10;
        char *next = (char*)malloc(next_len_est);
        size_t pos = 0;

        for (size_t j = 0; j < curr_len; ) {
            char d = curr[j];
            size_t k = j + 1;
            while (k < curr_len && curr[k] == d) ++k;
            size_t run_len = k - j;

            /* append digit */
            next[pos++] = d;

            /* append decimal representation of run_len */
            char buf[32];
            int len = 0;
            size_t temp = run_len;
            do {
                buf[len++] = '0' + (temp % 10);
                temp /= 10;
            } while (temp);
            for (int t = len - 1; t >= 0; --t)
                next[pos++] = buf[t];

            j = k;
        }
        next[pos] = '\0';

        free(curr);
        curr = next;
        curr_len = pos;
    }

    seq_t result = seq_from_str(curr);
    free(curr);
    return result;
}
