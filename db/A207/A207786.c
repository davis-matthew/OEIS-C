/*
 * A207786  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A207786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 25, u64 n <= 62, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS sequence A207786: Count of 4×n binary arrays avoiding
 *  horizontal patterns 000 and 011, and vertical patterns 000 and 101.
 *  The sequence is computed by a linear‑time dynamic programme
 *  that keeps the last two columns of the array as state.
 */
seq_t a(idx_t n)
{
    /* Pre‑computed list of vertical‑valid 4‑bit columns */
    static const int V[9] = {2,3,4,6,7,9,12,14,15};

    /* Transition lists: for each state (a,b) store allowed states (b,c) */
    static int init = 0;
    static int next_cnt[81];
    static int next_to[81][9];

    if (!init) {
        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 9; ++b) {
                int s = a * 9 + b;
                int cnt = 0;
                for (int c = 0; c < 9; ++c) {
                    /* check horizontal triples for all 4 rows */
                    int ok = 1;
                    for (int r = 0; r < 4; ++r) {
                        int bit_a = (V[a] >> r) & 1;
                        int bit_b = (V[b] >> r) & 1;
                        int bit_c = (V[c] >> r) & 1;
                        if ((bit_a == 0 && bit_b == 0 && bit_c == 0) ||
                            (bit_a == 0 && bit_b == 1 && bit_c == 1)) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        next_to[s][cnt++] = b * 9 + c;
                    }
                }
                next_cnt[s] = cnt;
            }
        }
        init = 1;
    }

    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    seq_t cur[81];
    seq_t nxt[81];

    /* initial state counts after two columns: each pair occurs once */
    for (int i = 0; i < 81; ++i) cur[i] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* zero next array */
        for (int i = 0; i < 81; ++i) nxt[i] = (seq_t)0;

        for (int s = 0; s < 81; ++s) {
            if (cur[s] == 0) continue;
            int cnt = next_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int t = next_to[s][k];
                nxt[t] += cur[s];
            }
        }

        /* copy nxt to cur for next iteration */
        for (int i = 0; i < 81; ++i) cur[i] = nxt[i];
    }

    /* sum over all final states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 81; ++i) result += cur[i];
    return result;
}
