/*
 * A234552  Number of (n+1) X (3+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Number of (n+1)×4 arrays over {0,…,6} with every 2×2 subblock
 *  having its diagonal sum differing from its antidiagonal sum by 3
 *  (absolute difference 3).  The computation uses a finite‑state
 *  recurrence:  rows are the states, two consecutive rows form an
 *  admissible 2×2 block iff for every column j
 *      (rowA[j] + rowB[j+1]) - (rowA[j+1] + rowB[j])  ∈ {+3,–3}.
 *
 *  Transition rule:  for each j the difference between consecutive
 *  entries of rowB must be equal to the difference of rowA plus ±3.
 *  This yields at most 56 possible successors for every row.
 *
 *  The dynamic programming count for a(n) is obtained by starting with
 *  all 2401 possible top rows (weight 1) and performing n transitions
 *  using the precomputed adjacency.  The total weight after n steps
 *  equals a(n).  Complexity: O(n × 2401) operations; memory
 *  usage ≈ (2401 × 56 × 4 bytes) ≈ 0.5 MB plus two row buffers.
 */

#include <stdint.h>

/* constants */
enum { ROWS = 2401 };      /* 7^4 possible rows                     */
enum { DEG_MAX = 56 };     /* maximum possible successors per row    */

/* adjacency tables */
static int next_row[ROWS][DEG_MAX];
static unsigned char outdeg[ROWS];
static int adjacency_ready = 0;

/* encode a 4‑digit row (in base 7) into an integer 0..2400 */
static inline int encode_row(int a0, int a1, int a2, int a3)
{
    return ((a0 * 7 + a1) * 7 + a2) * 7 + a3;
}

/* build the adjacency list once */
static void build_adjacency(void)
{
    for (int idxA = 0; idxA < ROWS; ++idxA) {
        /* decode idxA to digits a0..a3 */
        int rem = idxA;
        int a0 = rem / 343;          rem %= 343;
        int a1 = rem / 49;           rem %= 49;
        int a2 = rem / 7;            int a3 = rem % 7;

        int d0 = a1 - a0;
        int d1 = a2 - a1;
        int d2 = a3 - a2;

        unsigned char cnt = 0;
        for (int b0 = 0; b0 <= 6; ++b0) {
            for (int mask = 0; mask < 8; ++mask) {
                int step0 = d0 + 3 * ((mask & 1) ? 1 : -1);
                int b1 = b0 + step0;
                if (b1 < 0 || b1 > 6) continue;

                int step1 = d1 + 3 * (((mask >> 1) & 1) ? 1 : -1);
                int b2 = b1 + step1;
                if (b2 < 0 || b2 > 6) continue;

                int step2 = d2 + 3 * (((mask >> 2) & 1) ? 1 : -1);
                int b3 = b2 + step2;
                if (b3 < 0 || b3 > 6) continue;

                int idxB = encode_row(b0, b1, b2, b3);
                next_row[idxA][cnt++] = idxB;
            }
        }
        outdeg[idxA] = cnt;
    }
    adjacency_ready = 1;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure adjacency is prepared */
    if (!adjacency_ready)
        build_adjacency();

    /* DP buffers, persistent across invocations */
    static seq_t cur[ROWS];
    static seq_t nxt[ROWS];

    /* init: every row can be the top row with weight 1 */
    for (int i = 0; i < ROWS; ++i)
        cur[i] = 1;

    /* perform n transitions (height n+1 rows) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next buffer */
        for (int i = 0; i < ROWS; ++i)
            nxt[i] = 0;

        /* propagate counts */
        for (int p = 0; p < ROWS; ++p) {
            seq_t val = cur[p];
            if (val == 0) continue;
            unsigned char deg = outdeg[p];
            for (int k = 0; k < deg; ++k) {
                int q = next_row[p][k];
                nxt[q] += val;
            }
        }
        /* swap cur and nxt by copying */
        for (int i = 0; i < ROWS; ++i)
            cur[i] = nxt[i];
    }

    /* sum all counts for bottom rows: the desired a(n) */
    seq_t total = 0;
    for (int i = 0; i < ROWS; ++i)
        total += cur[i];

    return total;
}
