/*
 * A203959  Number of (n+1)X3 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that rows
 *          of b(i,j) and columns of c(i,j) are lexicographically
 *          nondecreasing.
 * https://oeis.org/A203959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 30, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
    Computation of OEIS sequence A203959.
    The number of (n+1)×3 arrays with entries in {0,1,2} such that  
        • the rows of b(i,j)=a(i,j)+a(i,j-1) (0≤i≤n, 1≤j≤2) are
          lexicographically nondecreasing;
        • the column vectors of  
          c(i,j)=a(i,j)+a(i-1,j) (1≤i≤n, 0≤j≤2) are 
          lexicographically nondecreasing (i.e. column 0 ≤ column 1 ≤ column 2).
    The implementation is an O(n) dynamic programming enumerating all
    feasible rows.  Each DP state consists of the previous row (27
    possibilities) and the comparison status of the three column pairs
    (3 bits, 0 = equal so far, 1 = ordered as j<k).  The DP proceeds
    through the rows from i=0 to i=n and counts the number of arrays.
*/

#include <stddef.h>
#include <stdint.h>

/* Provide the 3‑digit base‑3 enumeration of all 27 rows (a[3] ∈ {0,1,2}^3). */
static const uint8_t rows[27][3] = {
    {0,0,0},{0,0,1},{0,0,2},
    {0,1,0},{0,1,1},{0,1,2},
    {0,2,0},{0,2,1},{0,2,2},
    {1,0,0},{1,0,1},{1,0,2},
    {1,1,0},{1,1,1},{1,1,2},
    {1,2,0},{1,2,1},{1,2,2},
    {2,0,0},{2,0,1},{2,0,2},
    {2,1,0},{2,1,1},{2,1,2},
    {2,2,0},{2,2,1},{2,2,2}
};

/* Number of states: 27 rows × 8 bit‑mask possibilities */
#define NUM_STATES 216

seq_t a(idx_t n)
{
    if (n == 0) {               /* (1×3) arrays – all 3³ possibilities */
        return (seq_t)27;
    }

    /* DP tables */
    seq_t cur[NUM_STATES];
    seq_t nxt[NUM_STATES];

    /* initialise DP with all possible row 0 (no constraints yet) */
    for (idx_t i = 0; i < NUM_STATES; ++i) cur[i] = (seq_t)0;
    for (idx_t r = 0; r < 27; ++r)           /* bit‑mask 0 : all columns equal so far */
        cur[(size_t)r * 8] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        for (idx_t i = 0; i < NUM_STATES; ++i) nxt[i] = (seq_t)0;

        for (idx_t s = 0; s < NUM_STATES; ++s) {
            seq_t ways = cur[s];
            if (ways == (seq_t)0) continue;

            /* decode previous state */
            uint8_t prev_row = (uint8_t)(s / 8);
            uint8_t bits    = (uint8_t)(s % 8);

            uint8_t a0 = rows[prev_row][0];
            uint8_t a1 = rows[prev_row][1];
            uint8_t a2 = rows[prev_row][2];

            uint8_t bprev1 = a0 + a1;            /* b(i,1) */
            uint8_t bprev2 = a1 + a2;            /* b(i,2) */

            /* try all possible next rows */
            for (uint8_t nr = 0; nr < 27; ++nr) {
                uint8_t c0 = rows[nr][0];
                uint8_t c1 = rows[nr][1];
                uint8_t c2 = rows[nr][2];

                uint8_t bnew1 = c0 + c1;          /* b(i+1,1) */
                uint8_t bnew2 = c1 + c2;          /* b(i+1,2) */

                /* lexicographic nondecreasing of b rows */
                if (bnew1 < bprev1 || (bnew1 == bprev1 && bnew2 < bprev2))
                    continue;

                /* compute c(i+1,j) */
                uint8_t ccol0 = a0 + c0;
                uint8_t ccol1 = a1 + c1;
                uint8_t ccol2 = a2 + c2;

                uint8_t newbits = bits;

                /* pair (0,1) */
                if ((bits & 1) == 0) {
                    if (ccol0 > ccol1) continue;
                    if (ccol0 < ccol1) newbits |= 1;
                }
                /* pair (0,2) */
                if ((bits & 2) == 0) {
                    if (ccol0 > ccol2) continue;
                    if (ccol0 < ccol2) newbits |= 2;
                }
                /* pair (1,2) */
                if ((bits & 4) == 0) {
                    if (ccol1 > ccol2) continue;
                    if (ccol1 < ccol2) newbits |= 4;
                }

                /* register transition */
                size_t ns = (size_t)nr * 8 + newbits;
                nxt[ns] += ways;
            }
        }

        /* swap tables */
        for (idx_t i = 0; i < NUM_STATES; ++i) cur[i] = nxt[i];
    }

    /* sum all final states (columns may still be equal, which is allowed) */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < NUM_STATES; ++i) {
        total += cur[i];
    }
    return total;
}
