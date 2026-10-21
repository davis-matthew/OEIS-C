/*
 * A235957  Number of (n+1)X(3+1) 0..1 arrays with the difference of the upper
 *          and lower median value of each 2X2 subblock in lexicographically
 *          nondecreasing order columnwise and nonincreasing rowwise.
 * https://oeis.org/A235957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×4 binary arrays whose 2×2 subblock *
 *            upper–lower median differences form a lexicographically
 *            nondecreasing * sequence columnwise and a nonincreasing sequence
 *            rowwise. * * The sequence satisfies the empirical linear
 *            recurrence of order 18 * (empirical coefficients are o...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Transition tables are computed once, then reused for every call. */
static uint16_t trans[128][16];   /* next state indices */
static uint8_t  trans_cnt[128];   /* number of transitions per state */
static bool     trans_ready = false;

/* Build the transition graph. */
static void init_trans(void)
{
    if (trans_ready)
        return;
    for (uint32_t state = 0; state < 128; ++state)
    {
        uint8_t prev_row = (uint8_t)(state >> 3);   /* 4 bits */
        uint8_t last_d   = (uint8_t)(state & 7u);   /* 3 bits */
        uint8_t cnt = 0;
        for (uint8_t next_row = 0; next_row < 16; ++next_row)
        {
            /* Compute diff vector d (3 bits). */
            uint8_t d = 0;
            for (uint8_t j = 0; j < 3; ++j)
            {
                uint8_t sum =
                    ((prev_row >> j)   & 1u) +
                    ((prev_row >> (j+1)) & 1u) +
                    ((next_row >> j)   & 1u) +
                    ((next_row >> (j+1)) & 1u);
                uint8_t bit = (sum == 2u) ? 1u : 0u;
                d |= bit << j;
            }

            /* Nonincreasing order within the vector: d0>=d1>=d2. */
            uint8_t d0 = (d >> 0) & 1u;
            uint8_t d1 = (d >> 1) & 1u;
            uint8_t d2 = (d >> 2) & 1u;
            if (!((d0 >= d1) && (d1 >= d2)))
                continue;

            /* Nondecreasing across rows: d[j] >= last_d[j]. */
            bool ok = true;
            for (uint8_t j = 0; j < 3; ++j)
            {
                if ( ((d >> j) & 1u) < ((last_d >> j) & 1u) )
                {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                continue;

            /* Valid transition. */
            uint16_t next_state = ((uint16_t)next_row << 3) | d;
            trans[state][cnt++] = next_state;
        }
        trans_cnt[state] = cnt;
    }
    trans_ready = true;
}

/* Count arrays of size (n+1) × 4 satisfying the constraints. */
seq_t a(idx_t n)
{
    /* n is the number of transitions (row pairs). For the first
     * row there is no previous row, so we start with last_d = 0. */
    init_trans();

    /* DP arrays holding counts for each state. */
    seq_t cur[128];
    seq_t nxt[128];

    /* Initialize to zero. */
    for (uint32_t i = 0; i < 128; ++i) cur[i] = (seq_t)0;

    /* First row can be any of 16 patterns, last_d=0. */
    for (uint8_t r = 0; r < 16; ++r)
    {
        uint16_t state = ((uint16_t)r << 3); /* last_d is 0 */
        cur[state] = cur[state] + (seq_t)1;
    }

    /* Process each transition (there are n of them). */
    for (idx_t step = 0; step < n; ++step)
    {
        for (uint32_t i = 0; i < 128; ++i) nxt[i] = (seq_t)0;
        for (uint32_t s = 0; s < 128; ++s)
        {
            if (cur[s] == (seq_t)0)
                continue;
            seq_t val = cur[s];
            for (uint8_t k = 0; k < trans_cnt[s]; ++k)
            {
                uint16_t ns = trans[s][k];
                nxt[ns] = nxt[ns] + val;
            }
        }
        /* Copy nxt to cur for next iteration. */
        for (uint32_t i = 0; i < 128; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all counts – any final state represents a valid array. */
    seq_t total = (seq_t)0;
    for (uint32_t i = 0; i < 128; ++i)
        total = total + cur[i];

    return total;
}
