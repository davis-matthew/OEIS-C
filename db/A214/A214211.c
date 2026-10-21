/*
 * A214211  Doubled Fibonacci word: the A003842 sequence replacing 1 with 1,1
 *          and 2 with 2,2.
 * https://oeis.org/A214211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth term of the doubled Fibonacci word, i.e. the
 *            Fibonacci word over alphabet {1,2} (A003842) indexed by
 *            floor(n/2)
 * Range:     u8 n <= 190, u16 n <= 190, u32 n <= 190, u64 n <= 190,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Returns the value at index n in A214211. */
seq_t a(idx_t n)
{
    /* Convert index to size_t for array operations. */
    const size_t k = (size_t)(n >> 1);      /* floor(n/2) */

    /* Static cache holding the last two Fibonacci words. */
    static seq_t *prev = NULL;     /* F(k-1) */
    static size_t prev_len = 0;
    static seq_t *cur  = NULL;     /* F(k)   */
    static size_t cur_len  = 0;

    /* Initialise the first two Fibonacci words (F0 = [1], F1 = [1,2]). */
    if (cur_len == 0) {
        prev_len = 1;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev[0] = 1;                     /* F0 */

        cur_len = 2;
        cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        cur[0] = 1;
        cur[1] = 2;                      /* F1 */
    }

    /* Grow the Fibonacci word until it covers index k. */
    while (cur_len <= k) {
        size_t next_len = cur_len + prev_len;
        seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!next) abort();              /* Allocation failure */

        /* Copy cur and prev into next. */
        for (size_t i = 0; i < cur_len; ++i)
            next[i] = cur[i];
        for (size_t i = 0; i < prev_len; ++i)
            next[cur_len + i] = prev[i];

        /* Advance the window: prev becomes cur, cur becomes next. */
        free(prev);
        prev = cur;
        prev_len = cur_len;

        cur = next;
        cur_len = next_len;
    }

    /* Return the desired term.  Each term of the doubled word equals
       the corresponding term of the Fibonacci word. */
    return cur[k];
}
