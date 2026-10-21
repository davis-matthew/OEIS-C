/*
 * A236896  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A236896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} in which for every
 *            2×2 subblock the maximum plus the upper median is the same for
 *            all subblocks
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   max4, upper_median4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over rows.  The array
   consists of (n+1) rows; we process the rows sequentially and keep
   only the last row and the common sum of all 2×2 subblocks seen so far.
   The state space has size 27 × 5 (27 possible rows, 5 possible sums
   0..4).  For each pair of successive rows we pre‑compute the two
   candidate sums (columns 0–1 and 1–2).  A pair is valid only when
   both sums are equal; in that case the common value is that equal
   sum.  The DP transitions preserve this common value from one row to
   the next.  The algorithm runs in O(n·27²) time and O(27·5) space. */

#include <stdint.h>
#include "../utils/max4.h"
#include "../utils/upper_median4.h"

enum { ROWS = 27 };          /* 3^3 possible rows of length 3 */
enum { SUMS = 5 };           /* possible values of (max + upper median) */

/* All 27 rows in lexicographic order.  Row i corresponds to the
   base‑3 representation of i. */
static const int row_vals[ROWS][3] = {
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

/* pair_sums[i][j][0] = (max+upper median) for columns 0‑1
   of the 2×2 subblock formed by row i above row j.
   pair_sums[i][j][1] = same for columns 1‑2. */
static int pair_sums[ROWS][ROWS][2];
static int sums_initialized = 0;

static void init_sums(void)
{
    if(sums_initialized) return;
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<ROWS;j++){
            int a0=row_vals[i][0], a1=row_vals[i][1], a2=row_vals[i][2];
            int b0=row_vals[j][0], b1=row_vals[j][1], b2=row_vals[j][2];
            int s1 = max4(a0,a1,b0,b1) + upper_median4(a0,a1,b0,b1);
            int s2 = max4(a1,a2,b1,b2) + upper_median4(a1,a2,b1,b2);
            pair_sums[i][j][0] = s1;
            pair_sums[i][j][1] = s2;
        }
    }
    sums_initialized = 1;
}

seq_t a(idx_t n)
{
    if(n < 1) return 0;
    init_sums();

    /* dp[row][sum] = number of ways to reach a state ending with
       'row' (the last row) having common sum 'sum' for all 2×2 subblocks
       processed so far.  Two arrays are used to roll along the rows. */
    seq_t dp[ROWS][SUMS] = {0};
    seq_t next[ROWS][SUMS] = {0};

    /* Initialise for the first two rows */
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<ROWS;j++){
            int s1 = pair_sums[i][j][0];
            int s2 = pair_sums[i][j][1];
            if(s1 == s2){
                dp[j][s1] = dp[j][s1] + 1;
            }
        }
    }

    /* Process following rows.  We already had 2 rows; need n-1 more. */
    for(idx_t step=0; step+1 < n; step++){
        /* clear next */
        for(int r=0;r<ROWS;r++)
            for(int s=0;s<SUMS;s++)
                next[r][s] = 0;

        /* transition */
        for(int prev=0; prev<ROWS; prev++){
            for(int s=0; s<SUMS; s++){
                seq_t ways = dp[prev][s];
                if(ways == 0) continue;
                for(int cur=0; cur<ROWS; cur++){
                    int s1 = pair_sums[prev][cur][0];
                    int s2 = pair_sums[prev][cur][1];
                    if(s1 == s2 && s1 == s){
                        next[cur][s] = next[cur][s] + ways;
                    }
                }
            }
        }
        /* swap dp and next */
        for(int r=0;r<ROWS;r++)
            for(int s=0;s<SUMS;s++)
                dp[r][s] = next[r][s];
    }

    /* sum all dp entries */
    seq_t total = 0;
    for(int r=0;r<ROWS;r++)
        for(int s=0;s<SUMS;s++)
            total = total + dp[r][s];
    return total;
}
