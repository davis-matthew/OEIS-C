/*
 * A164210  Number of binary strings of length n with equal numbers of 00001
 *          and 11011 substrings.
 * https://oeis.org/A164210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of binary strings of length n with equal numbers of
 *   00001 and 11011 substrings.
 * (OEIS A164210)
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* the first 5 values are powers of two */
    if (n <= 4)
    {
        return (seq_t)1 << (size_t)n;
    }

    /* After the first 4 bits a 5‑bit window can first be formed.
     * Remaining bits to add: steps = n-4
     */
    idx_t steps      = n - 4;              /* number of remaining positions */
    size_t diff_center = steps;            /* index where difference=0 will be stored */
    size_t diff_span   = 2 * steps + 1;    /* total number of possible differences: [-steps,steps] */

    const size_t STATE_CNT = 16;           /* 4‑bit suffix */
    const size_t ARR_SIZE   = STATE_CNT * diff_span;

    /* two work arrays, cleared each iteration */
    seq_t *cur  = (seq_t *)malloc(ARR_SIZE * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(ARR_SIZE * sizeof(seq_t));
    if (!cur || !next) { return 0; }      /* allocation failed */
    memset(cur, 0, ARR_SIZE * sizeof(seq_t));
    memset(next, 0, ARR_SIZE * sizeof(seq_t));

    /* initial states: after the first 4 bits each 4‑bit pattern occurs exactly once,
     * difference is zero for all of them. */
    const size_t offset = diff_center;      /* index corresponding to difference 0 */
    for (size_t s = 0; s < STATE_CNT; ++s)
    {
        cur[s * diff_span + offset] = (seq_t)1;
    }

    /* transition table: next_state and difference change */
    int trans[STATE_CNT][2];
    int delta[STATE_CNT][2];
    const int PAT0 = 0b00001;   /* 1  */
    const int PAT1 = 0b11011;   /* 27 */

    for (int s = 0; s < 16; ++s)
    {
        for (int bit = 0; bit < 2; ++bit)
        {
            int ns = ((s << 1) | bit) & 0xF;     /* keep only last 4 bits */
            int w  = ((s << 1) | bit);           /* 5‑bit window */
            int d = 0;
            if (w == PAT0)        d =  1;
            else if (w == PAT1)   d = -1;
            trans[s][bit] = ns;
            delta[s][bit] = d;
        }
    }

    /* dynamic programming over the remaining bits */
    for (idx_t step = 0; step < steps; ++step)
    {
        size_t min_idx = diff_center - step;
        size_t max_idx = diff_center + step;     /* inclusive */

        for (size_t s = 0; s < STATE_CNT; ++s)
        {
            size_t base_cur   = s * diff_span;
            size_t base_next0 = trans[s][0] * diff_span;
            size_t base_next1 = trans[s][1] * diff_span;

            for (size_t d_idx = min_idx; d_idx <= max_idx; ++d_idx)
            {
                seq_t val = cur[base_cur + d_idx];
                if (val == 0) continue;

                /* append 0 */
                size_t nd0 = d_idx + delta[s][0];
                next[base_next0 + nd0] += val;

                /* append 1 */
                size_t nd1 = d_idx + delta[s][1];
                next[base_next1 + nd1] += val;
            }
        }

        /* prepare for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        memset(next, 0, ARR_SIZE * sizeof(seq_t));   /* clear the buffer we'll use next */
    }

    /* sum all states having difference == 0 */
    seq_t result = 0;
    for (size_t s = 0; s < STATE_CNT; ++s)
    {
        result += cur[s * diff_span + offset];
    }

    free(cur);
    free(next);
    return result;
}
