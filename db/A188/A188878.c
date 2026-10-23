/*
 * A188878  Number of 6Xn binary arrays without the pattern 0 0 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A188878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188878: Count of 6×n binary arrays with no horizontal or antidiagonal 3‑zero pattern. */

#include <stdint.h>
#include <stdbool.h>

#define SHIFT 6
#define STATES (1 << (2 * SHIFT))

/* Transition table entry */
typedef struct {
    uint16_t next[64]; /* maximum possible transitions for a state */
    uint8_t count;     /* how many of the above entries are valid */
} StateTrans;

/* Static tables */
static StateTrans trans[STATES];
static bool transitions_initialized = false;

/* Helper to set up the transition table */
static void init_transitions(void)
{
    for (int s = 0; s < STATES; ++s) {
        int prev2 = s >> SHIFT;
        int prev1 = s & 63;
        int forced = 0;

        /* Horizontal constraint: if both prev1 and prev2 are zero at a row */
        for (int r = 0; r < 6; ++r) {
            if (((prev1 >> r) & 1) == 0 && ((prev2 >> r) & 1) == 0)
                forced |= 1 << r;
        }

        /* Antidiagonal constraint: rows i, i+1, i+2 for i = 0..3 */
        for (int i = 0; i < 4; ++i) {
            if (((prev1 >> (i + 1)) & 1) == 0 && ((prev2 >> (i + 2)) & 1) == 0)
                forced |= 1 << i;
        }

        int cnt = 0;
        for (int newcol = 0; newcol < 64; ++newcol) {
            if ((newcol & forced) == forced) {
                trans[s].next[cnt++] = (prev1 << SHIFT) | newcol;
            }
        }
        trans[s].count = (uint8_t)cnt;
    }
    transitions_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return ((seq_t)1) << 6;     /* 2^(6*1) */
    if (n == 2) return ((seq_t)1) << 12;    /* 2^(6*2) */

    if (!transitions_initialized)
        init_transitions();

    static seq_t dp1[STATES];
    static seq_t dp2[STATES];
    seq_t *cur = dp1;
    seq_t *nxt = dp2;

    /* Initialize after 2 columns: one way for each pair of columns */
    for (int i = 0; i < STATES; ++i)
        cur[i] = 1;

    for (idx_t col = 3; col <= n; ++col) {
        /* zero nxt array */
        for (int i = 0; i < STATES; ++i)
            nxt[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0) continue;
            const StateTrans *st = &trans[s];
            for (int k = 0; k < st->count; ++k) {
                nxt[st->next[k]] += cnt;
            }
        }

        /* swap pointers for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all final states */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) {
        result += cur[i];
    }
    return result;
}
