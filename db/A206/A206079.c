/*
 * A206079  Number of (n+1)X3 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one.
 * https://oeis.org/A206079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206079: Number of (n+1)×3 0..3 arrays with the
   number of clockwise edge increases in every 2×2 subblock equal to one.
   Computation method: transfer-matrix enumeration.
   Each row of the array is a 3‑tuple (values 0–3).  Two consecutive rows
   are compatible iff for both adjacent column pairs the 2×2 block
   contains exactly one clockwise increase.
   The adjacency graph has 64 states and 960 directed edges.
   The number a(n) is the total number of walks of length n in this
   graph, i.e. the sum of all entries of M^n where M is the
   64×64 adjacency matrix.  The algorithm iterates the graph for n
   steps, using only additions on `seq_t`. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: check compatibility of two rows */
static bool rows_compatible(const uint8_t a[3], const uint8_t b[3]) {
    for (int c = 0; c < 2; ++c) {              /* two 2×2 blocks */
        uint8_t tl = a[c];
        uint8_t tr = a[c + 1];
        uint8_t bl = b[c];
        uint8_t br = b[c + 1];
        int inc = 0;
        inc += (tr > tl);
        inc += (br > tr);
        inc += (bl > br);
        inc += (tl > bl);
        if (inc != 1) return false;
    }
    return true;
}

/* Main function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Build adjacency lists once */
    static bool built = false;
    static uint8_t adj[64][64];
    static uint8_t deg[64];

    if (!built) {
        for (int src = 0; src < 64; ++src) {
            uint8_t sa[3];
            sa[0] = src / 16;
            sa[1] = (src / 4) % 4;
            sa[2] = src % 4;
            uint8_t d = 0;
            for (int dst = 0; dst < 64; ++dst) {
                uint8_t sb[3];
                sb[0] = dst / 16;
                sb[1] = (dst / 4) % 4;
                sb[2] = dst % 4;
                if (rows_compatible(sa, sb)) {
                    adj[src][d++] = dst;
                }
            }
            deg[src] = d;
        }
        built = true;
    }

    /* State vectors */
    static seq_t cur[64];
    static seq_t nxt[64];

    /* Initial vector: all ones (paths of length 0) */
    for (int i = 0; i < 64; ++i) cur[i] = 1;

    /* Iterate n steps */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) nxt[i] = 0;
        for (int src = 0; src < 64; ++src) {
            seq_t v = cur[src];
            if (!v) continue;
            uint8_t d = deg[src];
            for (uint8_t k = 0; k < d; ++k) {
                nxt[adj[src][k]] += v;
            }
        }
        for (int i = 0; i < 64; ++i) cur[i] = nxt[i];
    }

    /* Sum all entries to get a(n) */
    seq_t res = 0;
    for (int i = 0; i < 64; ++i) res += cur[i];
    return res;
}
