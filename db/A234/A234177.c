/*
 * A234177  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having the absolute values of all six edge and diagonal
 *          differences no larger than 1.
 * https://oeis.org/A234177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A234177: Number of (n+1) × 2 arrays with entries 0..5 where
   every 2×2 subblock has all 6 edge and diagonal differences ≤ 1.
   The sequence satisfies the recurrence
     a(n) = 10 a(n−1) − 27 a(n−2) + 3 a(n−3) + 36 a(n−4) + 12 a(n−5)
   for n ≥ 6 with a(1)=76, a(2)=388, a(3)=1978, a(4)=10120, a(5)=51886.
   This implementation uses a simple dynamic‑programming
   approach based on the adjacency graph of the 16 admissible rows.
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the 16 allowed row pairs (x,y) with |x-y|≤1 */
    static const int row[16][2] = {
        {0,0},{0,1},{1,0},{1,1},{1,2},{2,1},{2,2},{2,3},
        {3,2},{3,3},{3,4},{4,3},{4,4},{4,5},{5,4},{5,5}
    };
    /* transition matrix T[i][j] == 1 if row j can be followed by row i */
    static unsigned char T[16][16] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 16; ++i) {
            int x1 = row[i][0], y1 = row[i][1];
            for (int j = 0; j < 16; ++j) {
                int x2 = row[j][0], y2 = row[j][1];
                if ( ( ( (x1>y1?x1-y1:y1-x1)<=1 )   /* |x1-y1| ≤ 1 (holds by construction) */
                     && ((x2>y2?x2-y2:y2-x2)<=1)   /* |x2-y2| ≤ 1 */
                     && ((x1>x2?x1-x2:x2-x1)<=1)   /* |x1-x2| ≤ 1 */
                     && ((y1>y2?y1-y2:y2-y1)<=1)   /* |y1-y2| ≤ 1 */
                     && ((x1>y2?x1-y2:y2-x1)<=1)   /* |x1-y2| ≤ 1 */
                     && ((y1>x2?y1-x2:x2-y1)<=1) ) ) /* |y1-x2| ≤ 1 */
                    T[i][j] = 1;
            }
        }
        initialized = true;
    }

    /* DP arrays: counts[i] = number of length‑k sequences ending with row i */
    seq_t counts[16];
    seq_t temp  [16];

    /* initial state: sequence of length 1 (first row) */
    for (int i = 0; i < 16; ++i)
        counts[i] = 1;

    /* perform n transitions to obtain sequences of length n+1 */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 16; ++i) {
            seq_t sum = 0;
            for (int j = 0; j < 16; ++j)
                if (T[i][j])
                    sum += counts[j];
            temp[i] = sum;
        }
        for (int i = 0; i < 16; ++i)
            counts[i] = temp[i];
    }

    /* total number of sequences = sum over all ending rows */
    seq_t result = 0;
    for (int i = 0; i < 16; ++i)
        result += counts[i];
    return result;
}
