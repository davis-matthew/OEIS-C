/*
 * A022481  Describe previous term from the right (method B - initial term is
 *          1).
 * https://oeis.org/A022481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) – “Describe previous term from the right” (Method B)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; from a(k) produce a(k+1) by reading a(k) from right to left,
   describing each run of equal digits as: digit followed by its run length. */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n < 1, return 0 to avoid UB */
        return (seq_t)0;

    /* start with the first term "1" */
    char *cur = (char *)malloc(2);
    cur[0] = '1';
    cur[1] = '\0';
    size_t cur_len = 1;

    for (idx_t k = 2; k <= n; ++k) {
        size_t i = cur_len;
        size_t out_len = 0;
        size_t estimate = cur_len * 2 + 10;           /* buffer estimate */
        char *next = (char *)malloc(estimate);

        while (i > 0) {
            char d = cur[--i];          /* current digit from right end */
            size_t run = 1;
            while (i > 0 && cur[i-1] == d) {   /* count run length */
                run++;
                i--;
            }

            /* ensure space for digit and its count */
            if (out_len >= estimate - 1) {
                estimate <<= 1;
                next = (char *)realloc(next, estimate);
            }
            next[out_len++] = d;                /* digit */

            /* convert run length to decimal string */
            char rbuf[32];
            size_t rl = run;
            int ri = 0;
            do {
                rbuf[ri++] = '0' + (rl % 10);
                rl /= 10;
            } while (rl > 0);

            for (int j = ri - 1; j >= 0; --j) {
                if (out_len >= estimate - 1) {
                    estimate <<= 1;
                    next = (char *)realloc(next, estimate);
                }
                next[out_len++] = rbuf[j];
            }
        }

        next[out_len] = '\0';            /* terminate the new term */
        free(cur);                       /* discard previous term */
        cur = next;
        cur_len = out_len;               /* new length for next iteration */
    }

    seq_t result = seq_from_str(cur);  /* convert final string to seq_t */
    free(cur);
    return result;
}
