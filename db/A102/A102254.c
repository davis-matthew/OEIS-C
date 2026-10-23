/*
 * A102254  Begin with 3, multiply each digit by 2, keeping the memory of the
 *          groupings of the preceding digits.
 * https://oeis.org/A102254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is obtained by starting with the string "3" and, n‑1 times,
 *            replacing * each decimal digit d by the decimal representation
 *            of d*2, concatenating the * results. This generates OEIS A102254
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The build supplies: typedef ... seq_t; typedef ... idx_t; */
/* Example definitions (not part of this file):
 *   typedef unsigned long long seq_t;
 *   typedef uint64_t idx_t;
 */

seq_t a(idx_t n)
{
    /* offset is 1: a(1) = 3 */
    if (n < 1)
        return (seq_t)0;                       /* undefined, but graceful */

    /* Handle the trivial first term directly. */
    if (n == 1)
        return (seq_t)3;

    /* Allocate buffer for the current term’s decimal digits. */
    size_t cur_len = 1;
    char *cur = (char *)malloc(cur_len + 1);   /* +1 for the null terminator used only for safety */
    if (!cur)
        return (seq_t)0;                       /* allocation failed */

    cur[0] = '3';
    cur[1] = '\0';                             /* not strictly needed */

    /* Apply the digit‑by‑digit doubling process n‑1 times. */
    for (idx_t step = 1; step < n; ++step) {
        /* In the worst case every digit doubles its length. */
        size_t next_len_est = cur_len * 2;
        char *next = (char *)malloc(next_len_est + 1);  /* +1 for null terminator in the loop */
        if (!next) {
            free(cur);
            return (seq_t)0;                       /* allocation failure */
        }

        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            unsigned d = (unsigned)(cur[i] - '0');
            unsigned v = d * 2;
            if (v >= 10) {
                next[pos++] = '0' + (v / 10);
                next[pos++] = '0' + (v % 10);
            } else {
                next[pos++] = '0' + v;
            }
        }
        next[pos] = '\0';                         /* keep string null-terminated for safety */
        free(cur);
        cur = next;
        cur_len = pos;
    }

    /* Convert the final decimal string to a seq_t value. */
    seq_t result = 0;
    for (size_t i = 0; i < cur_len; ++i) {
        result = result * (seq_t)10 + (cur[i] - '0');
    }

    free(cur);
    return result;
}
