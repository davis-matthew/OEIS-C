/*
 * A181272  Number of 4 X n binary matrices M with rows in strictly increasing
 *          order and rows of M*transpose(M) (mod 2) in strictly increasing
 *          order.
 * https://oeis.org/A181272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: DP over columns, tracking relative ordering of the 4 rows of M
 * and the 4×4 parity matrix G = M·Mᵀ (mod 2).  The DP has 2¹³ states
 * (order of adjacent pairs in M + parity of all entries of G).  For each
 * n the DP recomputes from the neutral state, so the function works
 * independently of the order of calls. */

#include <stdint.h>
#include <stdbool.h>

#define STATE_PAR_BITS   10          /* r0,r1,r2,r3,p01,p02,p03,p12,p13,p23 */
#define STATE_ORDER_BITS 3           /* bits for the three adjacent‑row decisions */
#define TOTAL_BITS       (STATE_PAR_BITS + STATE_ORDER_BITS)
#define NUM_STATES       (1 << TOTAL_BITS)
#define TRANS_DENSE      16          /* possible column vectors (2⁴) */
#define INVALID_STATE    0xFFFFU    /* sentinel for invalid transitions */

static bool   initialized = false;
static seq_t dp[NUM_STATES];
static seq_t ndp[NUM_STATES];
static uint16_t trans[NUM_STATES][TRANS_DENSE];
static bool good_par[1 << STATE_PAR_BITS];
static uint16_t delta_par[TRANS_DENSE];

/* Precompute transition table and good_par. */
static void init_transition(void)
{
    /* Delta parity contributed by each column vector. */
    for (int v = 0; v < TRANS_DENSE; ++v) {
        unsigned b0 = (v >> 0) & 1;
        unsigned b1 = (v >> 1) & 1;
        unsigned b2 = (v >> 2) & 1;
        unsigned b3 = (v >> 3) & 1;
        uint16_t m = 0;
        m |= b0 << 0;                 /* r0   */
        m |= b1 << 1;                 /* r1   */
        m |= b2 << 2;                 /* r2   */
        m |= b3 << 3;                 /* r3   */
        m |= (b0 & b1) << 4;          /* p01  */
        m |= (b0 & b2) << 5;          /* p02  */
        m |= (b0 & b3) << 6;          /* p03  */
        m |= (b1 & b2) << 7;          /* p12  */
        m |= (b1 & b3) << 8;          /* p13  */
        m |= (b2 & b3) << 9;          /* p23  */
        delta_par[v] = m;
    }

    /* Good_par: rows of G increasing. */
    for (int par = 0; par < (1 << STATE_PAR_BITS); ++par) {
        unsigned r0 = (par >> 0) & 1;
        unsigned r1 = (par >> 1) & 1;
        unsigned r2 = (par >> 2) & 1;
        unsigned r3 = (par >> 3) & 1;
        unsigned p01 = (par >> 4) & 1;
        unsigned p02 = (par >> 5) & 1;
        unsigned p03 = (par >> 6) & 1;
        unsigned p12 = (par >> 7) & 1;
        unsigned p13 = (par >> 8) & 1;
        unsigned p23 = (par >> 9) & 1;

        unsigned g0 = (r0 << 3) | (p01 << 2) | (p02 << 1) | p03;
        unsigned g1 = (p01 << 3) | (r1 << 2) | (p12 << 1) | p13;
        unsigned g2 = (p02 << 3) | (p12 << 2) | (r2 << 1) | p23;
        unsigned g3 = (p03 << 3) | (p13 << 2) | (p23 << 1) | r3;

        good_par[par] =
            (g0 < g1) && (g1 < g2) && (g2 < g3);
    }

    /* Transition table. */
    for (int s = 0; s < NUM_STATES; ++s) {
        unsigned order = (s >> STATE_PAR_BITS) & 7; /* undecided/done bits */
        unsigned par   =  s & ((1 << STATE_PAR_BITS) - 1);

        for (int v = 0; v < TRANS_DENSE; ++v) {
            unsigned b0 = (v >> 0) & 1;
            unsigned b1 = (v >> 1) & 1;
            unsigned b2 = (v >> 2) & 1;
            unsigned b3 = (v >> 3) & 1;

            bool bad = false;
            unsigned new_order = order;

            /* pair (0,1) */
            if (!(order & 1)) {
                if (b0 && !b1) { bad = true; }
                else if (!b0 && b1) new_order |= 1;
            }

            /* pair (1,2) */
            if (!(order & 2)) {
                if (b1 && !b2) { bad = true; }
                else if (!b1 && b2) new_order |= 2;
            }

            /* pair (2,3) */
            if (!(order & 4)) {
                if (b2 && !b3) { bad = true; }
                else if (!b2 && b3) new_order |= 4;
            }

            if (bad) {
                trans[s][v] = INVALID_STATE;
                continue;
            }

            unsigned new_par = (unsigned)(par ^ delta_par[v]);
            trans[s][v] = (uint16_t)((new_order << STATE_PAR_BITS) | new_par);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    if (!initialized) {
        init_transition();
        initialized = true;
    }

    /* Re‑initialise DP arrays for this call. */
    for (int i = 0; i < NUM_STATES; ++i) dp[i] = 0;
    dp[0] = 1;

    seq_t *cur = dp, *nxt = ndp;

    for (idx_t col = 0; col < n; ++col) {
        for (int i = 0; i < NUM_STATES; ++i) nxt[i] = 0;

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;

            for (int v = 0; v < TRANS_DENSE; ++v) {
                uint16_t ns = trans[s][v];
                if (ns == INVALID_STATE) continue;
                nxt[ns] += val;
            }
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t ans = 0;
    for (int s = 0; s < NUM_STATES; ++s) {
        unsigned order = (s >> STATE_PAR_BITS) & 7;
        if (order != 7) continue;
        unsigned par = s & ((1 << STATE_PAR_BITS) - 1);
        if (!good_par[par]) continue;
        ans += cur[s];
    }

    return ans;
}
