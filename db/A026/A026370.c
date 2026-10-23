/*
 * A026370  a(n) = a(m) if a(m) has already occurred exactly once and n =
 *          a(m)+2m+1, else a(n) = least positive integer that has not yet
 *          occurred.
 * https://oeis.org/A026370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(m) if a(m) has already occurred exactly once and * n =
 *            a(m)+2m+1, otherwise a(n) = least positive integer * that has
 *            not yet occurred. Offset: 1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t *seq      = NULL;   /* a[1..len]               */
    static seq_t *sched    = NULL;   /* scheduled[n] = value    */
    static size_t *cnt      = NULL;   /* count[value]             */
    static size_t seq_len       = 0;  /* number of values computed */
    static size_t seq_cap       = 0;
    static size_t sched_cap     = 0;
    static size_t cnt_cap       = 0;
    static seq_t next_missing   = 1;  /* smallest unused integer */

    /* Initialisation on first call */
    if (seq_len == 0) {
        /* Allocate small initial buffers */
        seq_cap   = 128;
        sched_cap = seq_cap;
        seq       = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        sched     = (seq_t *)calloc(sched_cap, sizeof(seq_t));

        cnt_cap   = 1024;
        cnt       = (size_t *)calloc(cnt_cap, sizeof(size_t));

        /* a(1)=1 */
        seq[1] = 1;
        seq_len = 1;
        cnt[1] = 1;

        /* schedule the first recurrence: n = a(1)+2*1+1 = 4 */
        idx_t target = (idx_t)1 + 2*1 + 1;      /* 4 */
        if (target >= sched_cap) {
            size_t new_cap = target + 128;
            sched = (seq_t *)realloc(sched, new_cap * sizeof(seq_t));
            memset(sched + sched_cap, 0, (new_cap - sched_cap) * sizeof(seq_t));
            sched_cap = new_cap;
        }
        sched[target] = 1;

        /* next_missing becomes 2 */
        next_missing = 2;
    }

    /* Compute terms up to n */
    while (seq_len < n) {
        idx_t idx = seq_len + 1;          /* next index to fill */

        /* Ensure seq and sched arrays large enough */
        if ((size_t)idx >= seq_cap) {
            size_t new_cap = seq_cap * 2 + 128;
            seq   = (seq_t *)realloc(seq,   new_cap * sizeof(seq_t));
            sched = (seq_t *)realloc(sched, new_cap * sizeof(seq_t));
            memset(sched + sched_cap, 0, (new_cap - sched_cap) * sizeof(seq_t));
            seq_cap   = new_cap;
            sched_cap = new_cap;
        }

        seq_t val;
        if (idx < sched_cap && sched[idx] != 0) {
            val = sched[idx];              /* recurrence triggers */
        } else {
            val = next_missing;            /* first unused integer */
        }

        seq[idx] = val;                     /* store the term */
        seq_len = idx;

        /* Update count array for val */
        size_t vidx = (size_t)val;          /* value as size_t index */
        if (vidx >= cnt_cap) {
            size_t new_cap = vidx + 128 + (cnt_cap >> 1);
            cnt = (size_t *)realloc(cnt, new_cap * sizeof(size_t));
            memset(cnt + cnt_cap, 0, (new_cap - cnt_cap) * sizeof(size_t));
            cnt_cap = new_cap;
        }
        cnt[vidx]++;

        /* If this value is now unique, schedule its recurrence */
        if (cnt[vidx] == 1) {
            idx_t target = (idx_t)val + 2*idx + 1;
            if (target >= sched_cap) {
                size_t new_cap = target + 128 + (sched_cap >> 1);
                sched = (seq_t *)realloc(sched, new_cap * sizeof(seq_t));
                memset(sched + sched_cap, 0, (new_cap - sched_cap) * sizeof(seq_t));
                sched_cap = new_cap;
            }
            if (sched[target] == 0)                 /* avoid overwriting */
                sched[target] = val;
        }

        /* Advance next_missing if it was just used */
        if (vidx == (size_t)next_missing) {
            do {
                next_missing++;
            } while ((size_t)next_missing < cnt_cap && cnt[(size_t)next_missing] > 0);
            /* Ensure cnt array large enough for the new next_missing */
            if ((size_t)next_missing >= cnt_cap) {
                size_t new_cap = (size_t)next_missing + 128 + (cnt_cap >> 1);
                cnt = (size_t *)realloc(cnt, new_cap * sizeof(size_t));
                memset(cnt + cnt_cap, 0, (new_cap - cnt_cap) * sizeof(size_t));
                cnt_cap = new_cap;
            }
        }
    }

    return seq[n];
}
