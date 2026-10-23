/*
 * A100053  Maximum run of white (or OFF) cells in generation n of the Rule 30
 *          elementary cellular automaton.
 * https://oeis.org/A100053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute maximum run of OFF (0) cells in generation n of Rule 30 cellular automaton. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Rule 30 transition table: pattern (left<<2 | center<<1 | right) -> next state */
static const uint8_t RULE30[8] = {0,1,1,1,1,0,0,0};

/* Cached sequence values a(0)...a(max_n) */
static seq_t *a_vals = NULL;          /* sequence values */
static size_t a_len  = 0;             /* allocated length */
static idx_t  a_max  = 0;             /* highest n computed */

/* Working buffers for cellular automaton */
static uint8_t *cur   = NULL;         /* current generation */
static uint8_t *next  = NULL;         /* next generation */
static size_t   cur_len = 0;          /* buffer length */
static idx_t    center  = 0;          /* array index corresponding to cell 0 */
static idx_t    cur_n   = 0;          /* generation currently stored in *cur */

/* Expand or allocate buffers so that they can hold generations up to max_n. */
static void expand_buffers(idx_t max_n)
{
    /* New length: 2*max_n + 5 guarantees room for two extra cells on each side. */
    idx_t  new_len  = max_n * 2 + 5;
    size_t new_len_s = (size_t)new_len;

    if (cur == NULL) {
        /* First allocation */
        cur = (uint8_t*)calloc(new_len_s, 1);
        next = (uint8_t*)calloc(new_len_s, 1);
        cur_len = new_len_s;
        center = max_n + 2;           /* index of logical cell 0 */
        cur[center] = 1;              /* single ON cell at generation 0 */
        cur_n = 0;
    } else if (new_len_s > cur_len) {
        /* Grow buffers */
        uint8_t *new_cur  = (uint8_t*)calloc(new_len_s, 1);
        uint8_t *new_next = (uint8_t*)calloc(new_len_s, 1);

        /* Shift old data to new center */
        idx_t shift = (max_n + 2) - center;   /* new_center - old_center */
        if (shift < 0) shift = 0;             /* safety, though not expected */

        for (size_t i = 0; i < cur_len; ++i) {
            size_t ni = i + (size_t)shift;
            if (ni < new_len_s) new_cur[ni] = cur[i];
        }

        free(cur);
        free(next);
        cur   = new_cur;
        next  = new_next;
        cur_len = new_len_s;
        center  = max_n + 2;
    }
    /* If new_len_s <= cur_len, buffers already large enough. */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Allocate or grow the a_vals array. */
    if (a_vals == NULL) {
        a_len = (size_t)n + 1;
        a_vals = (seq_t*)malloc(a_len * sizeof(seq_t));
        if (!a_vals) return (seq_t)0;
        a_vals[0] = 0;
        a_max = 0;
    } else if (a_len <= (size_t)n) {
        a_len = (size_t)n + 1;
        a_vals = (seq_t*)realloc(a_vals, a_len * sizeof(seq_t));
        if (!a_vals) return (seq_t)0;
    }

    /* Ensure buffers are large enough for required generations. */
    if (cur == NULL || (idx_t)cur_len/2 - 2 < n) {
        expand_buffers(n);
    }

    /* Simulate missing generations. */
    if (n > a_max) {
        /* Expand buffers once for the new maximum n before simulation. */
        expand_buffers(n);

        for (idx_t g = a_max + 1; g <= n; ++g) {
            idx_t start = center - g;
            idx_t end   = center + g;

            /* Compute next generation. */
            idx_t min1 = cur_len; /* large initial value */
            idx_t max1 = 0;
            for (idx_t pos = start; pos <= end; ++pos) {
                uint8_t left  = cur[pos - 1];
                uint8_t mid   = cur[pos];
                uint8_t right = cur[pos + 1];
                uint8_t pat   = (left << 2) | (mid << 1) | right;
                uint8_t val   = RULE30[pat];
                next[pos] = val;

                if (val == 1) {
                    if (pos < min1) min1 = pos;
                    if (pos > max1) max1 = pos;
                }
            }

            /* Count the longest run of zeros within the 1‑support. */
            seq_t max_run = 0;
            seq_t cur_run = 0;
            if (min1 <= max1) {
                for (idx_t pos = min1; pos <= max1; ++pos) {
                    if (next[pos] == 0) {
                        ++cur_run;
                    } else {
                        if (cur_run > max_run) max_run = cur_run;
                        cur_run = 0;
                    }
                }
                if (cur_run > max_run) max_run = cur_run;
            }
            a_vals[g] = max_run;

            /* Prepare for next iteration. */
            uint8_t *tmp = cur;
            cur = next;
            next = tmp;
            cur_n = g;
        }
        a_max = n;
    }

    return a_vals[n];
}
