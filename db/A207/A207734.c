/*
 * A207734  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207734: a(n) = # of 7×n binary arrays avoiding 000 and 010 horizontally
   and 011 and 101 vertically.  
   The algorithm uses a DP over the two last columns as states.
   States are pairs of admissible columns (31 choices each), so there are
   31² = 961 possible states.  For each step we transition to a new
   column that satisfies the vertical restriction with the previous
   column.  The DP is O(n·31²) which is fast for n up to 10 000.  */
#include <stddef.h>

/* --- precomputation of columns and adjacency --- */
static int  a207734_initialized = 0;
static uint8_t a207734_K;                     /* # admissible column masks (31) */
static uint8_t a207734_colMask[32];           /* admissible col masks */
static uint8_t a207734_needMask[32];          /* bits that must be 1 in next column */
static uint8_t a207734_adj[32][32];           /* adjacency lists for transitions */
static uint8_t a207734_adjCnt[32];            /* # neighbours for each column */

/* DP arrays for current and next step */
static seq_t a207734_curr[32 * 32];
static seq_t a207734_next[32 * 32];

static void a207734_init(void)
{
    if (a207734_initialized)
        return;
    a207734_initialized = 1;

    /* --- list admissible column masks (avoid vertical 011 and 101) --- */
    uint8_t mask;
    uint8_t k = 0;
    for (mask = 0; mask < 128; ++mask) {
        int good = 1;
        for (int r = 0; r <= 4; ++r) {
            int a = (mask >> r) & 1;
            int b = (mask >> (r + 1)) & 1;
            int c = (mask >> (r + 2)) & 1;
            if ((a == 0 && b == 1 && c == 1) || (a == 1 && b == 0 && c == 1)) {
                good = 0;
                break;
            }
        }
        if (good) {
            a207734_colMask[k] = mask;
            ++k;
        }
    }
    a207734_K = k;               /* k == 31 for this problem */

    /* --- compute masks that must appear in the next column --- */
    for (int i = 0; i < a207734_K; ++i) {
        a207734_needMask[i] = (~a207734_colMask[i]) & 0x7F;  /* only 7 bits used */
    }

    /* --- build adjacency lists --- */
    for (int i = 0; i < a207734_K; ++i) {
        uint8_t cnt = 0;
        for (int j = 0; j < a207734_K; ++j) {
            if ((a207734_colMask[j] & a207734_needMask[i]) == a207734_needMask[i]) {
                a207734_adj[i][cnt++] = (uint8_t)j;
            }
        }
        a207734_adjCnt[i] = cnt;
    }
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    a207734_init();            /* ensure tables are ready */

    if (n == 1)                /* single column: any admissible column works */
        return (seq_t)a207734_K;

    /* --- DP initialisation: for n==2 all pairs are allowed once --- */
    idx_t K = (idx_t)a207734_K;
    idx_t S = K * K;           /* number of states */
    seq_t *curr = a207734_curr;
    seq_t *next = a207734_next;

    for (idx_t pos = 0; pos < S; ++pos)
        curr[pos] = (seq_t)1;   /* state (i,j) counts = 1 */

    /* --------------------------------------------------------------------- */
    /* iterate columns 3 .. n (if n==2 the loop body is skipped)           */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear next array */
        for (idx_t pos = 0; pos < S; ++pos)
            next[pos] = (seq_t)0;

        /* transition from states (i,j) to (j,k) */
        for (idx_t i = 0; i < K; ++i) {
            uint8_t *neigh = a207734_adj[i];
            uint8_t neighCnt = a207734_adjCnt[i];
            for (idx_t j = 0; j < K; ++j) {
                seq_t ways = curr[i * K + j];
                if (!ways)
                    continue;
                for (idx_t t = 0; t < neighCnt; ++t) {
                    idx_t k = (idx_t)neigh[t];
                    next[j * K + k] += ways;
                }
            }
        }

        /* swap current and next for next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* --------------------------------------------------------------------- */
    /* sum all states after the last column to get the total number of
       arrays of width n. */
    seq_t total = (seq_t)0;
    for (idx_t pos = 0; pos < S; ++pos)
        total += curr[pos];

    return total;
}
