/*
 * A237120  Number of white areas in the graph of elementary cellular
 *          automaton with rule 150 at generation n.
 * https://oeis.org/A237120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of zero runs (contiguous white segments) in the
 *            nth row of the elementary cellular automaton rule 150, starting
 *            from a single black cell (value 1) at generation 0
 * Range:     u8 n <= 509, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 has a single cell of value 1 → no white areas. */
    if (n == 0)
        return (seq_t)0;

    /* Convert the index to a type safe for array operations. */
    const size_t N = (size_t)n;

    /* The length of row k is 2k+1.  Allocate buffers large enough for
       the final row plus a small safety margin. */
    const size_t max_len = 2 * N + 3;
    unsigned char *buf1 = (unsigned char *)calloc(max_len, sizeof(unsigned char));
    unsigned char *buf2 = (unsigned char *)calloc(max_len, sizeof(unsigned char));
    if (!buf1 || !buf2) {
        /* Allocation failed – return 0 (unlikely in the tests). */
        free(buf1);
        free(buf2);
        return (seq_t)0;
    }

    /* Initial row (generation 0): one cell of value 1 at position 0. */
    size_t len_prev = 1;
    buf1[0] = 1;
    unsigned char *prev = buf1;
    unsigned char *cur  = buf2;

    /* Iterate from generation 1 up to generation n. */
    for (size_t step = 0; step < N; ++step) {
        size_t len_cur = len_prev + 2;

        /* Compute next generation according to rule 150:
           cur[i] = prev[i-2] XOR prev[i-1] XOR prev[i] (out of range = 0). */
        for (size_t i = 0; i < len_cur; ++i) {
            unsigned char a = (i >= 2 && i - 2 < len_prev) ? prev[i - 2] : 0;
            unsigned char b = (i >= 1 && i - 1 < len_prev) ? prev[i - 1] : 0;
            unsigned char c = (i   < len_prev)                 ? prev[i]     : 0;
            cur[i] = a ^ b ^ c;
        }

        /* Prepare for next iteration. */
        len_prev = len_cur;
        unsigned char *tmp  = prev;
        prev = cur;
        cur  = tmp;
    }

    /* Count the number of maximal contiguous runs of zeros in the final row. */
    unsigned long long runs = 0;
    int in_run = 0;
    for (size_t i = 0; i < len_prev; ++i) {
        if (prev[i] == 0) {
            if (!in_run) {
                ++runs;
                in_run = 1;
            }
        } else {
            in_run = 0;
        }
    }

    /* Clean up. */
    free(buf1);
    free(buf2);

    return (seq_t)runs;
}
