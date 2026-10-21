/*
 * A203960  Number of (n+1)X4 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that rows
 *          of b(i,j) and columns of c(i,j) are lexicographically
 *          nondecreasing.
 * https://oeis.org/A203960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays aᵢⱼ∈{0,1,2} such that * - For
 *            each i, j>0: b(i,j)=a(i,j)+a(i,j-1). * The vectors b(i,1..3) are
 *            lexicographically nondecreasing with i. * - For each i>0, j:
 *            c(i,j)=a(i,j)+a(i-1,j). * The column vectors c(1..n,j) are
 *            lexicographically nondecreasing with j. * The valu...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* compile‑time constants */
#define NUM_PAT 81   /* 3⁴ possible rows */
#define NUM_MASK 64  /* 2⁶ possible equality states for 6 column pairs */

/* mapping of column pair (j,l) j<l to a bit index 0..5:
 *  (0,1)->0, (0,2)->1, (0,3)->2,
 *  (1,2)->3, (1,3)->4, (2,3)->5
 */
static const int pairBit[4][4] = {
    { -1,  0,  1,  2 },
    { -1, -1,  3,  4 },
    { -1, -1, -1,  5 },
    { -1, -1, -1, -1 }
};

/* global tables populated by init() */
static unsigned char pattern[NUM_PAT][4];          /* row pattern digits 0..2   */
static unsigned char bvec[NUM_PAT][3];            /* b(i,j) values 0..4       */
static unsigned char lessMask[NUM_PAT][NUM_PAT];
static unsigned char equalMask[NUM_PAT][NUM_PAT];
static unsigned char greaterMask[NUM_PAT][NUM_PAT];
static unsigned char rowOk[NUM_PAT][NUM_PAT];

static int init_done = 0;

static void init(void)
{
    /* generate all row patterns in base‑3 */
    for (int p = 0; p < NUM_PAT; ++p) {
        int x = p;
        for (int k = 3; k >= 0; --k) {
            pattern[p][k] = (unsigned char)(x % 3);
            x /= 3;
        }
        /* compute b vector for this pattern */
        bvec[p][0] = pattern[p][0] + pattern[p][1];
        bvec[p][1] = pattern[p][1] + pattern[p][2];
        bvec[p][2] = pattern[p][2] + pattern[p][3];
    }

    /* precompute row order and column masks */
    for (int p = 0; p < NUM_PAT; ++p) {
        for (int q = 0; q < NUM_PAT; ++q) {
            /* row lex order: b(p) <= b(q) */
            int cmp = 0;
            for (int k = 0; k < 3; ++k) {
                if (bvec[p][k] < bvec[q][k]) { cmp = -1; break; }
                if (bvec[p][k] > bvec[q][k]) { cmp =  1; break; }
            }
            rowOk[p][q] = (cmp <= 0);

            /* compute column sums s_j = pattern[p][j] + pattern[q][j] */
            unsigned char s[4];
            for (int j = 0; j < 4; ++j)
                s[j] = pattern[p][j] + pattern[q][j];   /* 0..4 */

            unsigned char lMask = 0, eMask = 0, gMask = 0;
            for (int j = 0; j < 4; ++j) {
                for (int l = j + 1; l < 4; ++l) {
                    int diff = (int)s[j] - (int)s[l];
                    int bit = pairBit[j][l];
                    if (diff < 0)          lMask |= (unsigned char)(1 << bit);
                    else if (diff > 0)     gMask |= (unsigned char)(1 << bit);
                    else                   eMask |= (unsigned char)(1 << bit);
                }
            }
            lessMask[p][q]   = lMask;
            equalMask[p][q]  = eMask;
            greaterMask[p][q] = gMask;
        }
    }
    init_done = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1 */

    if (!init_done) init();

    /* two DP layers */
    static seq_t dp[2][NUM_PAT][NUM_MASK];
    int cur = 0, nxt = 1;

    /* initialize dp for first row */
    for (int p = 0; p < NUM_PAT; ++p) {
        for (int m = 0; m < NUM_MASK; ++m)
            dp[cur][p][m] = 0;
        dp[cur][p][NUM_MASK - 1] = 1; /* all bits set – all columns equal so far */
    }

    /* perform n transitions (add rows 2..n+1) */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next layer */
        for (int p = 0; p < NUM_PAT; ++p)
            for (int m = 0; m < NUM_MASK; ++m)
                dp[nxt][p][m] = 0;

        /* transition */
        for (int p = 0; p < NUM_PAT; ++p) {
            for (int m = 0; m < NUM_MASK; ++m) {
                seq_t val = dp[cur][p][m];
                if (val == 0) continue;
                for (int q = 0; q < NUM_PAT; ++q) {
                    if (!rowOk[p][q]) continue;
                    unsigned char gMask = greaterMask[p][q];
                    if (m & gMask) continue;           /* forbidden > relation */
                    unsigned char newMask = m & equalMask[p][q];
                    dp[nxt][q][newMask] += val;
                }
            }
        }
        /* swap layers */
        cur ^= 1;
        nxt ^= 1;
    }

    /* sum all remaining states */
    seq_t ans = 0;
    for (int p = 0; p < NUM_PAT; ++p)
        for (int m = 0; m < NUM_MASK; ++m)
            ans += dp[cur][p][m];

    return ans;
}
