/*
 * A036108  A summarize Fibonacci sequence starting with a(0)=2 and a(1)=0:
 *          summarize the previous two terms!.
 * https://oeis.org/A036108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 2
   a(1) = 0
   For n >= 2,
     let S = decimal string of a(n‑1) concatenated with a(n‑2).
     Count the occurrences of each decimal digit d (0–9) in S, giving a count c_d.
     Then a(n) is the decimal concatenation of the pairs (c_d , d) for all
     digits d with c_d > 0, taken in descending order of d.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)0;

    /* digit counts for the previous two terms */
    unsigned int cnt_prev2[10] = {0}, cnt_prev1[10] = {0}, cnt_cur[10] = {0}, cnt_comb[10];

    /* term values */
    seq_t term_prev2 = (seq_t)2;
    seq_t term_prev1 = (seq_t)0;
    seq_t term_cur   = 0;

    /* initialize counts for term(0) and term(1) */
    cnt_prev2[2]   = 1;   /* term 0 = "2" */
    cnt_prev1[0]   = 1;   /* term 1 = "0" */

    for (idx_t i = 2; i <= n; ++i) {
        /* combine counts of the two previous terms */
        for (int d = 0; d < 10; ++d) {
            cnt_comb[d] = cnt_prev1[d] + cnt_prev2[d];
        }

        /* build string for a(i) and compute its digit counts */
        size_t buf_cap = 64;
        char *buf = (char *)malloc(buf_cap);
        if (!buf) return (seq_t)0;          /* out‑of‑memory fallback */
        size_t pos = 0;
        memset(cnt_cur, 0, sizeof(cnt_cur));

        for (int d = 9; d >= 0; --d) {
            unsigned int c = cnt_comb[d];
            if (c == 0) continue;

            /* convert count to decimal string */
            char tmp[32];
            int len = snprintf(tmp, sizeof(tmp), "%u", c);

            /* ensure buffer capacity */
            if (pos + len + 1 > buf_cap) {
                buf_cap = (buf_cap + len + 10) * 2;
                char *new_buf = (char *)realloc(buf, buf_cap);
                if (!new_buf) { free(buf); return (seq_t)0; }
                buf = new_buf;
            }

            /* append count */
            memcpy(buf + pos, tmp, len);
            pos += len;

            /* update digit counts for the count part */
            for (int k = 0; k < len; ++k) {
                cnt_cur[tmp[k] - '0']++;
            }

            /* append the digit itself */
            buf[pos++] = '0' + d;
            cnt_cur[d]++;          /* digit d contributes once */
        }

        buf[pos] = '\0';          /* null terminate */

        term_cur = seq_from_str(buf);
        free(buf);

        /* shift the terms and their counts */
        memcpy(cnt_prev2, cnt_prev1, sizeof(cnt_prev1));
        memcpy(cnt_prev1, cnt_cur,  sizeof(cnt_cur));
        term_prev2 = term_prev1;
        term_prev1 = term_cur;
    }

    return term_cur;
}
