/*
 * A216190  Abelian complexity function of tribonacci word (A080843).
 * https://oeis.org/A216190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Abelian complexity of the Tribonacci word (A216190)
   a(n) = number of distinct Parikh vectors of factors of length n
   in the fixed point of the morphism 0→01, 1→02, 2→0.
   The algorithm generates a sufficiently long prefix of the word
   and counts distinct local Parikh vectors by a sliding window. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Use a generous factor to ensure that the generated prefix
       contains every factor of length n.  Empirically, 20×n+20
       is sufficient for all n ≤ 10 000; memory consumption stays
       well below a few megabytes. */
    const size_t N      = (size_t)n;
    const size_t needed = N * 20 + 20;             /* target length */
    const size_t alloc  = needed * 2 + 10;         /* buffer capacity */

    unsigned char *buf1 = (unsigned char *)malloc(alloc);
    unsigned char *buf2 = (unsigned char *)malloc(alloc);
    if (!buf1 || !buf2) {
        free(buf1);
        free(buf2);
        return 0;          /* allocation failure – safe fallback */
    }

    /* Build the fixed point of σ until we have at least 'needed' symbols */
    unsigned char *cur  = buf1;
    unsigned char *next = buf2;
    size_t cur_len = 1;              /* start with the word "0" */
    cur[0] = 0;

    while (cur_len < needed) {
        size_t next_len = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            unsigned char c = cur[i];
            if (c == 0) {
                /* σ(0) = 01 */
                next[next_len++] = 0;
                next[next_len++] = 1;
            } else if (c == 1) {
                /* σ(1) = 02 */
                next[next_len++] = 0;
                next[next_len++] = 2;
            } else {                /* c == 2, σ(2) = 0 */
                next[next_len++] = 0;
            }
        }
        /* Swap buffers for the next iteration */
        unsigned char *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = next_len;
    }

    /* Sliding window over the first cur_len symbols to collect
       distinct Parikh vectors of length N. */
    int cnt[3] = {0, 0, 0};   /* counts of 0,1,2 in the current window */

    /* Initialize the first window */
    for (size_t i = 0; i < N; ++i)
        ++cnt[cur[i]];

    /* Store unique vectors – at most 7 distinct vectors exist. */
    struct { int a, b, c; } uniq[8];
    size_t uniq_cnt = 1;
    uniq[0].a = cnt[0];
    uniq[0].b = cnt[1];
    uniq[0].c = cnt[2];

    /* Slide the window through the buffer */
    for (size_t i = 1; i + N <= cur_len; ++i) {
        /* Remove the leftmost symbol of the previous window */
        --cnt[cur[i - 1]];
        /* Add the new rightmost symbol */
        ++cnt[cur[i + N - 1]];

        /* Check if this Parikh vector was already observed */
        int present = 0;
        for (size_t j = 0; j < uniq_cnt; ++j) {
            if (uniq[j].a == cnt[0] &&
                uniq[j].b == cnt[1] &&
                uniq[j].c == cnt[2]) {
                present = 1;
                break;
            }
        }

        if (!present) {
            uniq[uniq_cnt].a = cnt[0];
            uniq[uniq_cnt].b = cnt[1];
            uniq[uniq_cnt].c = cnt[2];
            ++uniq_cnt;
            if (uniq_cnt >= 8)          /* safety guard; all vectors seen */
                break;
        }
    }

    free(buf1);
    free(buf2);
    return (seq_t)uniq_cnt;
}
