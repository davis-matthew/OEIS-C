/*
 * A239151  Number of (n+1)X(2+1) 0..3 arrays with no element equal to all
 *          horizontal neighbors or unequal to all vertical neighbors, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A239151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence (order 14) a(n) = 4*a(n‑1) + 51*a(n‑2) - 84*a(n‑3) -
 *            529*a(n‑4) + 442*a(n‑5) + 1605*a(n‑6) + 1916*a(n‑7) -
 *            5423*a(n‑8) - 5800*a(n‑9) + 8753*a(n‑10) + 890*a(n‑11) -
 *            5088*a(n‑12) + 4464*a(n‑13) + 3456*a(n‑14) Initial values (for
 *            n=1…14): 2, 2, 55, 208, 2778, 17050, 166531, 1221727, 10652...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STATES_PER_MAX 512          /* 8 (unsat) * 64 (row bits) */
#define MAX_STATE  (4 * STATES_PER_MAX)   /* 4 possible maxSeen values */
#define MAX_MAX 3

static unsigned trans_next[MAX_STATE][64];
static seq_t      trans_cnt [MAX_STATE][64];
static unsigned   trans_used[MAX_STATE];
static int       trans_inited = 0;

static inline unsigned encode_row(uint8_t v0, uint8_t v1, uint8_t v2)
{
    return ((unsigned)v0 << 4) | ((unsigned)v1 << 2) | (unsigned)v2;
}

static void init_transitions(void)
{
    if (trans_inited) return;
    static int valid[64];
    for (int v0 = 0; v0 < 4; ++v0)
        for (int v1 = 0; v1 < 4; ++v1)
            for (int v2 = 0; v2 < 4; ++v2) {
                int code = (v0 << 4) | (v1 << 2) | v2;
                valid[code] = (v0 != v1 && v2 != v1);
            }

    for (unsigned state = 0; state < MAX_STATE; ++state) {
        unsigned maxSeen   = state >> 9;          /* 2 bits */
        unsigned unsatMask = (state >> 6) & 7;     /* 3 bits */
        unsigned rowVal    = state & 63;          /* 6 bits */

        unsigned pv0 = (rowVal >> 4) & 3;
        unsigned pv1 = (rowVal >> 2) & 3;
        unsigned pv2 = rowVal & 3;
        unsigned prev[3] = {pv0, pv1, pv2};

        /* For each column, determine if value is forced or free */
        int forced[3];
        unsigned opt[3][4];
        int optcnt[3];
        for (int j = 0; j < 3; ++j) {
            forced[j] = (unsatMask >> j) & 1;
            if (forced[j]) {
                opt[j][0] = prev[j];
                optcnt[j] = 1;
            } else {
                optcnt[j] = 4;
                opt[j][0] = 0;
                opt[j][1] = 1;
                opt[j][2] = 2;
                opt[j][3] = 3;
            }
        }

        unsigned curMax = maxSeen;
        for (int i0 = 0; i0 < optcnt[0]; ++i0) {
            unsigned v0 = opt[0][i0];
            if (v0 > curMax + 1u) continue;
            unsigned m1 = v0 > curMax ? v0 : curMax;
            for (int i1 = 0; i1 < optcnt[1]; ++i1) {
                unsigned v1 = opt[1][i1];
                if (v1 > m1 + 1u) continue;
                unsigned m2 = v1 > m1 ? v1 : m1;
                for (int i2 = 0; i2 < optcnt[2]; ++i2) {
                    unsigned v2 = opt[2][i2];
                    if (v2 > m2 + 1u) continue;
                    unsigned m3 = v2 > m2 ? v2 : m2;

                    unsigned code = (v0 << 4) | (v1 << 2) | v2;
                    if (!valid[code]) continue;

                    unsigned nextMask = 0;
                    nextMask |= (v0 != pv0) << 0;
                    nextMask |= (v1 != pv1) << 1;
                    nextMask |= (v2 != pv2) << 2;
                    unsigned nextRowVal = encode_row((uint8_t)v0,
                                                     (uint8_t)v1,
                                                     (uint8_t)v2);
                    unsigned nextState = (m3 << 9) | (nextMask << 6) | nextRowVal;

                    /* Store transition */
                    unsigned k;
                    for (k = 0; k < trans_used[state]; ++k)
                        if (trans_next[state][k] == nextState) break;
                    if (k == trans_used[state]) {
                        trans_next[state][k]     = nextState;
                        trans_cnt[state][k]      = 1;
                        trans_used[state] += 1;
                    } else {
                        trans_cnt[state][k] += 1;
                    }
                }
            }
        }
    }

    trans_inited = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_transitions();

    static seq_t cur[MAX_STATE];
    static seq_t nxt[MAX_STATE];

    memset(cur, 0, sizeof(cur));

    /* --- Initialise for row 0 ------------------------------------ */
    /* Only two possible first rows: (0,1,0) and (0,1,2) */
    unsigned init1 = (1 << 9) | (7 << 6) | encode_row(0,1,0);   /* maxSeen=1 */
    unsigned init2 = (2 << 9) | (7 << 6) | encode_row(0,1,2);   /* maxSeen=2 */

    cur[init1] = 1;
    cur[init2] = 1;

    /* --- Process remaining rows (total rows = n+1, so n further rows) */
    for (unsigned r = 1; r <= n; ++r) {
        memset(nxt, 0, sizeof(nxt));
        for (unsigned s = 0; s < MAX_STATE; ++s) {
            if (cur[s] == 0) continue;
            for (unsigned t = 0; t < trans_used[s]; ++t) {
                unsigned ns = trans_next[s][t];
                nxt[ns] += cur[s] * trans_cnt[s][t];
            }
        }
        /* swap cur and nxt */
        memcpy(cur, nxt, sizeof(cur));
    }

    /* --- Sum states with unsatisfied vertical mask == 0 (last row satisfied) */
    seq_t res = 0;
    for (unsigned s = 0; s < MAX_STATE; ++s) {
        if ( ((s >> 6) & 7) == 0 )
            res += cur[s];
    }
    return res;
}
