/*
 * A189612  Number of n X 4 binary arrays without the pattern 0 1 0
 *          diagonally, antidiagonally or horizontally.
 * https://oeis.org/A189612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over rows. * Initial
 *            values: a(1)=12, a(2)=144
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts n×4 binary arrays avoiding the pattern
 * 010 horizontally, along the main diagonal and along the anti‑diagonal.
 * The DP state is a pair of the last two rows (each 4 bits).  */

#include <stddef.h>
#include <stdint.h>

/* Allowed 4‑bit rows (no horizontal 010). */
static const int allowedRows[12] = {
    0, 1, 3, 6, 7, 8, 9, 11, 12, 13, 14, 15
};

/* Mapping from a 4‑bit value to its index in allowedRows
 * (or -1 if the value is disallowed).  */
static const int rowToIdx[16] = {
    0, 1, -1, 2, -1, -1, 3, 4,
    5, 6, -1, 7, 8, 9,10,11
};

/* transitions[state][k] holds a possible next row index.
 * transCount[state] gives the number of transitions.  */
static int transitions[144][12];
static int transCount[144] = {0};

static int init_done = 0;

/* Check whether three consecutive rows (by indices) are compatible
 * (i.e., no 010 pattern arises in any direction).  */
static int rows_compatible(int a_idx, int b_idx, int c_idx)
{
    int a = allowedRows[a_idx];
    int b = allowedRows[b_idx];
    int c = allowedRows[c_idx];

    /* Main diagonal: (a[0],b[1],c[2]) and (a[1],b[2],c[3]) */
    if ((((a >> 0) & 1) == 0) &&
        (((b >> 1) & 1) == 1) &&
        (((c >> 2) & 1) == 0)) return 0;
    if ((((a >> 1) & 1) == 0) &&
        (((b >> 2) & 1) == 1) &&
        (((c >> 3) & 1) == 0)) return 0;

    /* Anti‑diagonal: (a[2],b[1],c[0]) and (a[3],b[2],c[1]) */
    if ((((a >> 2) & 1) == 0) &&
        (((b >> 1) & 1) == 1) &&
        (((c >> 0) & 1) == 0)) return 0;
    if ((((a >> 3) & 1) == 0) &&
        (((b >> 2) & 1) == 1) &&
        (((c >> 1) & 1) == 0)) return 0;

    return 1;
}

/* Populate the transition tables once.  */
static void init_tables(void)
{
    if (init_done) return;
    for (int a_idx = 0; a_idx < 12; ++a_idx) {
        for (int b_idx = 0; b_idx < 12; ++b_idx) {
            int state = a_idx * 12 + b_idx;
            int cnt = 0;
            for (int c_idx = 0; c_idx < 12; ++c_idx) {
                if (rows_compatible(a_idx, b_idx, c_idx)) {
                    transitions[state][cnt++] = c_idx;
                }
            }
            transCount[state] = cnt;
        }
    }
    init_done = 1;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* unused, offset is 1 */
    if (n == 1) return (seq_t)12;          /* base case */
    if (n == 2) return (seq_t)144;         /* base case: 12×12 */

    init_tables();

    static seq_t dpPrev[144];
    static seq_t dpNext[144];

    /* initialize counts for sequences of length 2: each allowed pair */
    for (int i = 0; i < 144; ++i) dpPrev[i] = (seq_t)1;

    for (idx_t len = 3; len <= n; ++len) {
        /* reset dpNext */
        for (int i = 0; i < 144; ++i) dpNext[i] = (seq_t)0;

        for (int st = 0; st < 144; ++st) {
            seq_t ways = dpPrev[st];
            if (!ways) continue;
            int cnt = transCount[st];
            for (int k = 0; k < cnt; ++k) {
                int c_idx = transitions[st][k];
                int newSt = (st % 12) * 12 + c_idx;  /* (b,c) */
                dpNext[newSt] = dpNext[newSt] + ways;
            }
        }
        /* swap dpPrev and dpNext */
        for (int i = 0; i < 144; ++i) dpPrev[i] = dpNext[i];
    }

    /* sum all counts for length n */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 144; ++i) result = result + dpPrev[i];
    return result;
}
