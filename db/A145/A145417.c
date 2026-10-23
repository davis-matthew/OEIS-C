/*
 * A145417  Number of 2-factors in P_8 X P_n.
 * https://oeis.org/A145417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of 2‑factors in P_8 × P_n computed by a transfer‑matrix
 *            counting algorithm. The algorithm uses a state that records, for
 *            every row i (0 ≤ i < 8), whether there is a horizontal edge from
 *            the current column to the next one. For each column all vertical
 *            edge configurations are enumerated and...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* small transition record */
struct transrec {
    uint8_t next;   /* next state mask */
    uint8_t cnt;    /* how many vertical configurations lead to this transition */
};

static struct transrec transition[256][128];
static uint8_t transition_len[256] = {0};
static int transition_ready = 0;

/* Build transition table once */
static void build_transition(void)
{
    if (transition_ready) return;
    for (int prev = 0; prev < 256; ++prev) {
        uint32_t cnt[256] = {0};

        /* enumerate all 2^7 vertical edge configurations */
        for (int vert = 0; vert < 128; ++vert) {
            int vdeg[8] = {0};

            /* count vertical edges incident to each vertex */
            for (int i = 0; i < 7; ++i) {
                if (vert & (1 << i)) {
                    vdeg[i]   += 1;
                    vdeg[i+1] += 1;
                }
            }

            int ok = 1;
            int next = 0;

            for (int i = 0; i < 8; ++i) {
                int l = (prev >> i) & 1;           /* left edge present? */
                int r = 2 - l - vdeg[i];           /* required right edge? */
                if (r < 0 || r > 1) {
                    ok = 0;
                    break;
                }
                if (r) next |= (1 << i);
            }

            if (ok) cnt[next] += 1;
        }

        /* compact non‑zero transitions */
        uint8_t pos = 0;
        for (int next = 0; next < 256; ++next) {
            if (cnt[next]) {
                transition[prev][pos].next = (uint8_t)next;
                transition[prev][pos].cnt  = (uint8_t)cnt[next];
                ++pos;
            }
        }
        transition_len[prev] = pos;
    }
    transition_ready = 1;
}

seq_t a(idx_t n)
{
    build_transition();                     /* ensure transitions ready */

    seq_t cur[256];
    seq_t nxt[256];
    for (int i = 0; i < 256; ++i) cur[i] = 0;/* clear current vector      */
    cur[0] = 1;                              /* start with left edges none */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;   /* clear next vector    */

        for (int prev = 0; prev < 256; ++prev) {
            seq_t val = cur[prev];
            if (val == 0) continue;              /* skip empty states     */

            uint8_t len = transition_len[prev];
            for (uint8_t k = 0; k < len; ++k) {
                int  next = transition[prev][k].next;
                int  mult = transition[prev][k].cnt;
                nxt[next] += val * (seq_t)mult;   /* multiply by small int */
            }
        }

        /* copy next back into cur for next iteration */
        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    return cur[0];   /* number of 2‑factors of P_8 × P_n */
}
