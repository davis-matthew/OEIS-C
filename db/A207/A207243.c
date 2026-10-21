/*
 * A207243  Number of 3Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6 a(2) = 36 For n ≥ 3 the values are computed by a DP
 *            over columns that enforces the horizontal forbidden patterns
 *            000,011 and the vertical forbidden patterns 001,010. The
 *            recurrence is implemented below
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 29, u64 n <= 67, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first two values directly. */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;

    /* Allowed column patterns (vertical constraints: forbids 001,010). */
    static const int cols[6] = {0, 3, 4, 5, 6, 7};

    /* Pre‑compute whether a triple of columns is horizontally admissible. */
    static bool tripleAllowed[6][6][6];
    static bool init = false;
    if (!init) {
        for (int cp = 0; cp < 6; ++cp) {
            for (int cq = 0; cq < 6; ++cq) {
                for (int cr = 0; cr < 6; ++cr) {
                    bool ok = true;
                    for (int row = 0; row < 3 && ok; ++row) {
                        int b0 = (cols[cp] >> (2 - row)) & 1;
                        int b1 = (cols[cq] >> (2 - row)) & 1;
                        int b2 = (cols[cr] >> (2 - row)) & 1;
                        int trip = (b0 << 2) | (b1 << 1) | b2;
                        if (trip == 0 || trip == 3) ok = false; /* 000 or 011 forbidden */
                    }
                    tripleAllowed[cp][cq][cr] = ok;
                }
            }
        }
        init = true;
    }

    /* DP arrays for the last two columns. States are encoded as cp*6+cpq. */
    seq_t cur[36], nxt[36];

    /* Base for n = 2: all 36 pairs of allowed columns count as one way each. */
    for (int cp = 0; cp < 6; ++cp)
        for (int cq = 0; cq < 6; ++cq)
            cur[cp * 6 + cq] = (seq_t)1;

    /* Process columns 3 … n. */
    for (idx_t col = 3; col <= n; ++col) {
        /* Initialize new DP array to zero. */
        for (int i = 0; i < 36; ++i) nxt[i] = (seq_t)0;

        /* Transition: from state (cp, cq) to (cq, cr) if triple allowed. */
        for (int cp = 0; cp < 6; ++cp) {
            for (int cq = 0; cq < 6; ++cq) {
                seq_t ways = cur[cp * 6 + cq];
                if (ways == (seq_t)0) continue;
                for (int cr = 0; cr < 6; ++cr) {
                    if (tripleAllowed[cp][cq][cr]) {
                        nxt[cq * 6 + cr] += ways;
                    }
                }
            }
        }

        /* Move to next state. */
        for (int i = 0; i < 36; ++i) cur[i] = nxt[i];
    }

    /* The total number of arrays is the sum of all end states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 36; ++i) total += cur[i];
    return total;
}
