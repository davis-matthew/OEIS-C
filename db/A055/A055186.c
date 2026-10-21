/*
 * A055186  Cumulative counting sequence: method A
 *          (adjective-before-noun)-pairs with first term 0.
 * https://oeis.org/A055186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1019, u16 n <= 1019, u32 n <= 1019, u64 n <= 1019,
 *            u128 n <= 1019, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterative construction of OEIS A055186 via cumulative counting. */

#include <stdlib.h>

/* The function may be called with any order of indices; it memoises
   the sequence in dynamic arrays that grow as needed. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset 1, defensive */

    /* static state used for memoisation */
    static seq_t *seq  = NULL;
    static idx_t  seq_len    = 0;
    static idx_t  seq_cap    = 0;
    static idx_t *cnt  = NULL;
    static idx_t  cnt_cap   = 0;
    static idx_t  cnt_max   = 0;  /* current maximum value in seq */

    /* one‑time initialization */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;
        seq[0] = (seq_t)0;                 /* start value */

        cnt_cap   = 1;
        cnt = (idx_t*)calloc(cnt_cap, sizeof(idx_t));
        cnt[0] = 1;                        /* frequency of 0 */
        cnt_max   = 0;
    }

    /* generate more terms until we reach n */
    while (seq_len < n) {
        idx_t old_len = seq_len;            /* start position of new block */

        /* append inventory for all integers 0 … cnt_max */
        for (idx_t i = 0; i <= cnt_max; i++) {
            seq_t count = (seq_t)cnt[i];

            /* grow seq array if necessary */
            if (seq_len + 2 > seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = count;
            seq[seq_len++] = (seq_t)i;
        }

        /* update frequencies for the newly appended values */
        for (idx_t idx = old_len; idx < seq_len; idx++) {
            /* value is a seq_t; it is safe to cast to idx_t because
               values grow slowly (see the conjecture in the entry).   */
            idx_t v = (idx_t)seq[idx];

            /* extend count array if the value is out of bounds */
            if (v >= cnt_cap) {
                idx_t new_cap = cnt_cap ? cnt_cap * 2 : 1;
                while (new_cap <= v)
                    new_cap *= 2;
                idx_t *tmp = (idx_t*)realloc(cnt, new_cap * sizeof(idx_t));
                for (idx_t j = cnt_cap; j < new_cap; j++)
                    tmp[j] = 0;
                cnt     = tmp;
                cnt_cap = new_cap;
            }

            cnt[v]++;                       /* increment frequency */
            if (v > cnt_max) cnt_max = v;   /* possibly new maximum */
        }
    }

    return seq[n - 1];
}
