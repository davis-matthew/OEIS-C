/*
 * A208103  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Dynamic programming over last two rows. Let S = {1,3,4,5,6,7} be
 *            the 6 allowed row patterns (3‑bit numbers) that avoid 000 and
 *            010 horizontally. Let DP[i][a][b] = number of matrices of height
 *            i whose last row is a and second‑last row is b. For i = 2,
 *            DP[2][a][b] = 1 for all a,b∈S. Transition (for...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int rows[6] = {1, 3, 4, 5, 6, 7};

static inline bool valid_triplet(int a, int b, int c)
{
    /* a = second‑last, b = last, c = new row (all 3‑bit patterns) */
    for (int j = 0; j < 3; ++j) {
        int bit_a = (a >> j) & 1;
        int bit_b = (b >> j) & 1;
        int bit_c = (c >> j) & 1;
        if (bit_a == 0 && bit_b == 1 && bit_c == 1) return false; /* 011 */
        if (bit_a == 1 && bit_b == 1 && bit_c == 0) return false; /* 110 */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined offset, safe default */
    if (n == 1) return (seq_t)6;     /* only horizontal constraints */
    if (n == 2) return (seq_t)36;    /* two rows, no vertical constraints */

    /* DP arrays for two consecutive layers */
    seq_t dpPrev[6][6] = {0};
    seq_t dpNext[6][6] = {0};

    /* Initialize for height 2: all pairwise combinations are allowed */
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            dpPrev[a][b] = 1;

    /* Iterate from height 2 up to n-1, building height +1 */
    for (idx_t step = 2; step < n; ++step) {
        /* zero dpNext */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dpNext[i][j] = 0;

        for (int last = 0; last < 6; ++last) {
            for (int prev = 0; prev < 6; ++prev) {
                seq_t cur = dpPrev[last][prev];
                if (cur == 0) continue;
                for (int nxt = 0; nxt < 6; ++nxt) {
                    if (valid_triplet(rows[prev], rows[last], rows[nxt]))
                        dpNext[nxt][last] += cur;
                }
            }
        }
        /* copy dpNext -> dpPrev for next iteration */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }

    /* Sum over all states for final height n */
    seq_t total = 0;
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            total += dpPrev[a][b];
    return total;
}
