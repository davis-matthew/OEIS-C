/*
 * A231954  Number of (n+1)X(5+1) 0..1 arrays with every element equal to some
 *          horizontal, vertical, diagonal or antidiagonal neighbor, with top
 *          left element zero.
 * https://oeis.org/A231954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute the number of (n+1)×6 binary arrays with each * element
 *            having at least one equal horizontal, vertical, * diagonal or
 *            antidiagonal neighbour, and with the top‑left element * fixed to
 *            0. This is done by a dynamic program over rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes */
#include <stdbool.h>
#include <string.h>

/* State size: previous row mask (6 bits) + pending mask (6 bits) */
#define STATE_COUNT 4096        /* 2^(6+6) */
#define MAX_TRANS   64          /* for each state at most 64 possible next rows */

/* Transition structure */
typedef struct {
    uint8_t next_mask;     /* mask of the new row (6 bits) */
    uint8_t next_pending;  /* pending bits for the new row (6 bits) */
} Transition;

/* Precomputed transition tables */
static Transition trans[STATE_COUNT][MAX_TRANS];
static uint8_t     trans_cnt[STATE_COUNT];
static bool        trans_built = false;

/* Build transition table once */
static void build_transitions(void)
{
    for (int P = 0; P < 64; ++P)                 /* previous row */
    {
        for (int F = 0; F < 64; ++F)             /* pending bits of previous row */
        {
            int s = (F << 6) | P;
            uint8_t tc = 0;

            for (int C = 0; C < 64; ++C)         /* current row mask */
            {
                /* Check that all pending cells of the previous row
                   receive an equal neighbour in the current row */
                bool ok = true;
                for (int j = 0; j < 6 && ok; ++j)
                {
                    if (F & (1 << j))
                    {
                        int v = (P >> j) & 1;
                        bool sat = false;
                        if (j > 0 && ((C >> (j - 1)) & 1) == v) sat = true;
                        if (j < 5 && ((C >> (j + 1)) & 1) == v) sat = true;
                        if (((C >> j) & 1) == v)                sat = true;
                        if (!sat) ok = false;
                    }
                }
                if (!ok) continue;

                /* Compute pending bits for the new row */
                int pendingC = 0;
                for (int j = 0; j < 6; ++j)
                {
                    int vC = (C >> j) & 1;
                    bool sat = false;
                    /* horizontal neighbours in the current row */
                    if (j > 0 && ((C >> (j - 1)) & 1) == vC) sat = true;
                    if (j < 5 && ((C >> (j + 1)) & 1) == vC) sat = true;
                    /* vertical/diagonal neighbours from the previous row */
                    if (!sat)
                    {
                        if (((P >> j) & 1) == vC)                sat = true;
                        if (j > 0 && ((P >> (j - 1)) & 1) == vC) sat = true;
                        if (j < 5 && ((P >> (j + 1)) & 1) == vC) sat = true;
                    }
                    if (!sat) pendingC |= (1 << j);
                }

                trans[s][tc].next_mask     = (uint8_t)C;
                trans[s][tc].next_pending = (uint8_t)pendingC;
                ++tc;
            }
            trans_cnt[s] = tc;
        }
    }
    trans_built = true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!trans_built)
        build_transitions();

    /* DP arrays */
    static seq_t          cur[STATE_COUNT];
    static seq_t          next[STATE_COUNT];
    static uint16_t       cur_active[STATE_COUNT];
    static uint16_t       next_active[STATE_COUNT];
    int cur_count  = 0;
    int next_count = 0;

    /* initialise */
    memset(cur, 0, sizeof(cur));

    /* ----- first row (row 0) --------------------------------- */
    for (int C = 0; C < 64; ++C)
    {
        if (C & 1) continue;                 /* top‑left element must be 0 */

        /* pending bits for this row (horizontal neighbours only) */
        int pending = 0;
        for (int j = 0; j < 6; ++j)
        {
            int vC = (C >> j) & 1;
            bool sat = false;
            if (j > 0 && ((C >> (j - 1)) & 1) == vC) sat = true;
            if (j < 5 && ((C >> (j + 1)) & 1) == vC) sat = true;
            if (!sat) pending |= (1 << j);
        }

        int state = (pending << 6) | C;
        cur[state] = 1;
        cur_active[cur_count++] = (uint16_t)state;
    }

    /* process remaining rows (there are n more rows after the first) */
    for (idx_t r = 0; r < n; ++r)
    {
        memset(next, 0, sizeof(next));
        next_count = 0;

        for (int ai = 0; ai < cur_count; ++ai)
        {
            int state = cur_active[ai];
            seq_t val  = cur[state];
            uint8_t tcnt = trans_cnt[state];

            for (int ti = 0; ti < tcnt; ++ti)
            {
                const Transition *t = &trans[state][ti];
                int ns = (t->next_pending << 6) | t->next_mask;

                if (next[ns] == 0) next_active[next_count++] = (uint16_t)ns;
                next[ns] += val;
            }
        }

        memcpy(cur, next, sizeof(cur));
        cur_count = next_count;
        for (int i = 0; i < cur_count; ++i)
            cur_active[i] = next_active[i];
    }

    /* sum states with no pending bits (top bits 0) */
    seq_t result = 0;
    for (int state = 0; state < STATE_COUNT; ++state)
    {
        if ((state >> 6) == 0)
            result += cur[state];
    }
    return result;
}
