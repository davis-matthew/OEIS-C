/*
 * A207026  Number of 3Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of 3×n binary arrays avoiding horizontal patterns 000 and
 *            011 in each row, * and vertical patterns 001 and 101 within each
 *            column. * Computed by dynamic programming over the 64 possible
 *            states of the last two bits of each row
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 29, u64 n <= 67, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static int next_state[64][6];           /* next state for each possible transition */
static unsigned next_cnt[64];           /* number of transitions from a state          */
static seq_t  initdp[64];               /* number of ways to reach each state after 2 columns */
static int    built = 0;

static void build_transition_tables(void)
{
    if (built) return;
    built = 1;

    /* list of allowed column vectors (3 bits) */
    const int allowed[6] = {0,2,3,4,6,7};   /* all 3-bit values except 001 (1) and 101 (5) */
    const unsigned allowed_cnt = 6;

    /* build initdp: counts after 2 columns */
    for (unsigned i = 0; i < 64; ++i) initdp[i] = 0;
    for (unsigned i = 0; i < allowed_cnt; ++i) {
        for (unsigned j = 0; j < allowed_cnt; ++j) {
            int c1 = allowed[i];
            int c2 = allowed[j];
            int state = 0;
            for (int r = 0; r < 3; ++r) {
                int a = (c1 >> (2 - r)) & 1;   /* bit from column 1, older */
                int b = (c2 >> (2 - r)) & 1;   /* bit from column 2, newer */
                state |= (a << (2*r));
                state |= (b << (2*r + 1));
            }
            initdp[state] = initdp[state] + 1;      /* seq_t addition */
        }
    }

    /* build transition table */
    for (unsigned s = 0; s < 64; ++s) {
        next_cnt[s] = 0;
        for (unsigned cidx = 0; cidx < allowed_cnt; ++cidx) {
            int c = allowed[cidx];
            int new_state = 0;
            int ok = 1;
            for (int r = 0; r < 3; ++r) {
                int old_a = (s >> (2*r)) & 1;       /* older */
                int old_b = (s >> (2*r + 1)) & 1;   /* newer */
                int new_bit = (c >> (2 - r)) & 1;   /* upcoming bit for this row */
                int triple = (old_a << 2) | (old_b << 1) | new_bit;
                if (triple == 0 || triple == 0b011) { /* forbidden 000 or 011 */
                    ok = 0; break;
                }
                int shifted_a = old_b;              /* becomes older in new state */
                int shifted_b = new_bit;            /* becomes newer */
                new_state |= (shifted_a << (2*r));
                new_state |= (shifted_b << (2*r + 1));
            }
            if (ok) {
                next_state[s][next_cnt[s]++] = new_state;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;

    build_transition_tables();

    seq_t dp[64];
    seq_t dp_next[64];

    /* Initialize DP to state after two columns */
    for (unsigned i = 0; i < 64; ++i) dp[i] = initdp[i];

    /* DP for columns 3..n */
    for (idx_t col = 3; col <= n; ++col) {
        for (unsigned i = 0; i < 64; ++i) dp_next[i] = 0;
        for (unsigned s = 0; s < 64; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            for (unsigned t = 0; t < next_cnt[s]; ++t) {
                int ns = next_state[s][t];
                dp_next[ns] = dp_next[ns] + ways;  /* seq_t addition */
            }
        }
        /* copy dp_next back into dp */
        for (unsigned i = 0; i < 64; ++i) dp[i] = dp_next[i];
    }

    /* Sum all states to get total number for n columns */
    seq_t total = 0;
    for (unsigned i = 0; i < 64; ++i) total = total + dp[i];
    return total;
}
