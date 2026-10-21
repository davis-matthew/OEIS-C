/*
 * A203376  Number of (n+1)X7 0..1 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 binary arrays whose every 2×2 subblock
 *            has equal main‑diagonal elements or equal anti‑diagonal elements
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* adjacency table between 7‑bit rows */
    static bool allowed[128][128];
    static bool built = false;
    if (!built) {
        for (int r0 = 0; r0 < 128; ++r0) {
            for (int r1 = 0; r1 < 128; ++r1) {
                bool ok = true;
                /* check the 6 2×2 blocks that cross rows r0 and r1 */
                for (int c = 0; c < 6; ++c) {
                    uint8_t a = (r0 >> c) & 1;      /* bit c of row r0   */
                    uint8_t b = (r0 >> (c + 1)) & 1;/* bit c+1 of row r0 */
                    uint8_t c_bit = (r1 >> c) & 1;   /* bit c of row r1   */
                    uint8_t d = (r1 >> (c + 1)) & 1;/* bit c+1 of row r1 */
                    if (!((a == d) || (b == c_bit))) {
                        ok = false;
                        break;
                    }
                }
                allowed[r0][r1] = ok;
            }
        }
        built = true;
    }

    /* dynamic programming over the n row‑pairs */
    seq_t cur[128];
    seq_t nxt[128];
    for (int i = 0; i < 128; ++i)
        cur[i] = 1;                 /* start at any row */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 128; ++i)
            nxt[i] = 0;
        for (int from = 0; from < 128; ++from) {
            seq_t val = cur[from];
            if (val == 0) continue;
            for (int to = 0; to < 128; ++to) {
                if (allowed[from][to])
                    nxt[to] += val;
            }
        }
        for (int i = 0; i < 128; ++i)
            cur[i] = nxt[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 128; ++i)
        total += cur[i];
    return total;
}
