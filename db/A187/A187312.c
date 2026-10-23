/*
 * A187312  Half the number of (n+2)X6 binary arrays with each 3X3 subblock
 *          having sum 3, 4, 5 or 6.
 * https://oeis.org/A187312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½·# of (n+2)×6 binary arrays in which every 3×3 subblock
 *            * has a sum of 3, 4, 5 or 6. This implementation uses * dynamic
 *            programming over the last two rows. There are 2¹²=4096 * states
 *            (two 6‑bit rows) and at most 64 possible successor rows for *
 *            each state. The DP is recomputed up to the...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------- */
/* Cross‑language allocation helpers – usable in both C and C++. */
/* ------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Allocate an array of seq_t of length n. */
static inline seq_t *alloc_seq_arr(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Reallocate an existing seq_t array to new_cap elements. */
static inline seq_t *realloc_seq_arr(seq_t *old, size_t old_cap, size_t new_cap)
{
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (old)
    {
        size_t i;
        for (i = 0; i < old_cap && i < new_cap; ++i) tmp[i] = old[i];
        delete[] old;
    }
    return tmp;
#else
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* ------------------------------------------------------------------- */
/* Transition pre‑computation.  Two 6‑bit rows → next row candidates. */
/* ------------------------------------------------------------------- */
#define STATE_BITS 12
#define STATE_COUNT (1u << STATE_BITS)
#define MAX_TRANS   64

static uint8_t next_rows[STATE_COUNT][MAX_TRANS];
static uint8_t next_len[STATE_COUNT];
static bool transitions_inited = false;

static void init_transitions(void)
{
    if (transitions_inited) return;

    for (int a = 0; a < 64; ++a)
    {
        for (int b = 0; b < 64; ++b)
        {
            int state = (a << 6) | b;
            uint8_t cnt = 0;

            for (int r = 0; r < 64; ++r)
            {
                bool ok = true;

                /* Check the four 3×3 windows formed by rows a,b,r. */
                for (int w = 0; w < 4; ++w)
                {
                    unsigned int sum = __builtin_popcount((a >> w) & 7);
                    sum += __builtin_popcount((b >> w) & 7);
                    sum += __builtin_popcount((r >> w) & 7);
                    if (sum < 3 || sum > 6) { ok = false; break; }
                }

                if (ok) next_rows[state][cnt++] = (uint8_t)r;
            }
            next_len[state] = cnt;
        }
    }
    transitions_inited = true;
}

/* ------------------------------------------------------------------- */
/* DP and caching of the sequence terms. */
/* ------------------------------------------------------------------- */
static seq_t dp[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

static seq_t *a_values = NULL;      /* a_values[0] is a dummy. */
static size_t a_vals_len = 0;       /* Number of stored terms (incl. dummy). */
static size_t a_vals_cap = 0;       /* Capacity of a_values array. */

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* out of range – not used by the caller. */

    /* Initialise the transition table once. */
    if (!transitions_inited) init_transitions();

    /* Allocate cache lazily. */
    if (a_values == NULL)
    {
        a_vals_cap = 16;
        a_values  = alloc_seq_arr(a_vals_cap);
        if (!a_values) abort();          /* allocation failure. */
        a_values[0] = 0;                 /* dummy entry. */
        a_vals_len = 1;

        /* Initial DP – all 4096 state pairs are possible for two rows. */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 1;
    }

    /* Extend the cache until we have a(n). */
    while (a_vals_len <= n)
    {
        /* Clear the next‑state table. */
        memset(dp_next, 0, sizeof(dp_next));

        /* Transition step: propagate counts from each state. */
        for (int state = 0; state < STATE_COUNT; ++state)
        {
            seq_t cnt = dp[state];
            if (!cnt) continue;

            uint8_t len   = next_len[state];
            const uint8_t *succ = next_rows[state];
            uint8_t b = state & 63;               /* last row of current state. */

            for (uint8_t k = 0; k < len; ++k)
            {
                uint8_t r = succ[k];
                int new_state = (b << 6) | r;
                dp_next[new_state] += cnt;
            }
        }

        /* Prepare for the next height. */
        memcpy(dp, dp_next, sizeof(dp));

        /* Total number of arrays of current height. */
        seq_t total = 0;
        for (int i = 0; i < STATE_COUNT; ++i) total += dp[i];

        /* a(current_height-2) = total / 2.  Evenness is guaranteed. */
        seq_t val = total / (seq_t)2;

        /* Grow cache array if needed. */
        if (a_vals_len == a_vals_cap)
        {
            size_t new_cap = a_vals_cap * 2;
            seq_t *tmp = realloc_seq_arr(a_values, a_vals_cap, new_cap);
            if (!tmp) abort();
            a_values = tmp;
            a_vals_cap = new_cap;
        }

        /* Store the computed term. */
        a_values[a_vals_len++] = val;
    }

    return a_values[n];
}
