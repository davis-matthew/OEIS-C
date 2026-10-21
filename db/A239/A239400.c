/*
 * A239400  Number of nX3 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A239400:
   DP over rows; state is the column sums modulo 4.
   For each row, transitions are precomputed once. */
#include <stddef.h>   // for size_t

seq_t a(idx_t n)
{
    /* transition table: trans[s][t] = #ways to go from state s to t in one row */
    static seq_t trans[64][64];
    static int trans_ready = 0;

    if (!trans_ready) {
        for (int s = 0; s < 64; ++s) {
            int s0 = (s / 16) % 4;
            int s1 = (s / 4) % 4;
            int s2 = s % 4;
            for (int a0 = 0; a0 < 4; ++a0) {
                if (a0 == (1 + 0) % 4) continue;            /* left of column 0 */
                if (a0 == (2 + s0) % 4) continue;           /* above column 0 */
                int left1 = (0 + a0) % 4;
                for (int a1 = 0; a1 < 4; ++a1) {
                    if (a1 == (1 + left1) % 4) continue;    /* left of column 1 */
                    if (a1 == (2 + s1) % 4) continue;       /* above column 1 */
                    int left2 = (left1 + a1) % 4;
                    for (int a2 = 0; a2 < 4; ++a2) {
                        if (a2 == (1 + left2) % 4) continue;/* left of column 2 */
                        if (a2 == (2 + s2) % 4) continue;   /* above column 2 */
                        int ns0 = (s0 + a0) % 4;
                        int ns1 = (s1 + a1) % 4;
                        int ns2 = (s2 + a2) % 4;
                        int ns  = ns0 * 16 + ns1 * 4 + ns2;
                        trans[s][ns] += (seq_t)1;
                    }
                }
            }
        }
        trans_ready = 1;
    }

    /* DP over rows */
    seq_t counts[64] = {0};
    seq_t newcounts[64];
    counts[0] = (seq_t)1;          /* all column sums start at 0 */

    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < 64; ++i) newcounts[i] = (seq_t)0;
        for (int s = 0; s < 64; ++s) {
            if (counts[s] == (seq_t)0) continue;
            for (int t = 0; t < 64; ++t) {
                if (trans[s][t] == (seq_t)0) continue;
                newcounts[t] += counts[s] * trans[s][t];
            }
        }
        for (int i = 0; i < 64; ++i) counts[i] = newcounts[i];
    }

    /* sum over all final states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += counts[i];
    return total;
}
