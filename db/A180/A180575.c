/*
 * A180575  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          nodes at distance k in the prism graph C_n X P_3 with the edges of
 *          the outer cycle removed (called a web graph). Equivalently, the
 *          graph is obtained by attaching a pendant edge to each node of the
 *          outer cycle of the circular ladder...
 * https://oeis.org/A180575
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   DH
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A180575: triangle read by rows.
 *
 * The entry a(n) is the n-th term of the flattened triangle,
 * with offset 3.  The i-th row (i ≥ 3) contains
 * 2 + ⌊i/2⌋ entries, which are the numbers of unordered
 * vertex pairs in the web graph with outer cycle length i
 * that lie at distance k (k = 1, 2, …, ⌊i/2⌋+2).
 *
 * This implementation computes the desired entry by:
 *   1) locating the row and column corresponding to index n,
 *   2) evaluating the combinatorial count of pairs at distance k
 *      using a closed formula derived from the graph structure.
 *
 * Complexity: O(√n) per call for n up to 10 000 (≈150 loop iterations).
 */

#include <stdint.h>   /* for uint64_t, etc. */
#include "../utils/DH.h"

static inline seq_t pair_count(idx_t r, idx_t k)
{
    /* Number of unordered vertex pairs in row r at distance k */
    seq_t n_seq = (seq_t)r;
    idx_t a     = r / 2;                /* floor(r/2) */
    seq_t res   = 0;

    if (k == 1) {
        /* distance 1: edges inside middle/lower cycles + vertical rungs */
        res += (seq_t)2 * DH(r, 1) + (seq_t)2 * n_seq;   /* 2*DH(1) + 2*n */
    } else {
        /* distance ≥ 2 */
        if (k <= a)                  /* (1,1) and (2,2) vertex pairs */
            res += (seq_t)2 * DH(r, k);

        if (k >= 2 && k - 1 <= a)    /* (0,1) and (1,2) vertex pairs */
            res += (seq_t)4 * DH(r, k - 1);

        if (k >= 3 && k - 2 <= a)    /* (0,0) and (0,2) vertex pairs */
            res += (seq_t)3 * DH(r, k - 2);   /* DH + 2*DH */

        if (k == 2)                  /* i == j in layer 0–2 */
            res += n_seq;
    }

    return res;
}

seq_t a(idx_t n)
{
    /* a(n) for n ≥ 3; below offset behaviour is unspecified */
    if (n < 3) return 0;              /* arbitrary for undefined indices */

    const idx_t OFFSET = 3;            /* first valid term index */
    idx_t idxOff = n - OFFSET;        /* 0‑based index in the flattened list */

    /* Locate row r and column k */
    idx_t r  = 3;
    idx_t sum = 0;
    while (1) {
        idx_t rowSize = 2 + r / 2;     /* number of entries in row r */
        if (sum + rowSize > idxOff) break;
        sum += rowSize;
        ++r;
    }
    idx_t k = idxOff - sum + 1;        /* 1‑based position within the row */

    return pair_count(r, k);
}
