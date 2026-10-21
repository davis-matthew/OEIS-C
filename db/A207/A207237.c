/*
 * A207237  Number of nX3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays avoiding horizontal patterns
 *            000,011 and vertical patterns 001,010. Computed via a DP over
 *            the last two rows; the state space is the 6 allowed rows, and
 *            transitions are allowed only if every column satisfies the
 *            vertical restriction with the preceding two rows
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* allowed row patterns (as 3-bit numbers: bit2=left, bit1=mid, bit0=right) */
static const seq_t ALLOWED[6] = {(seq_t)1,  /* 001 */
                                 (seq_t)2,  /* 010 */
                                 (seq_t)4,  /* 100 */
                                 (seq_t)5,  /* 101 */
                                 (seq_t)6,  /* 110 */
                                 (seq_t)7   /* 111 */
                                };

/* precomputed bits of each allowed pattern */
static const unsigned char BIT[6][3] = {
    {0,0,1},   /* 001 */
    {0,1,0},   /* 010 */
    {1,0,0},   /* 100 */
    {1,0,1},   /* 101 */
    {1,1,0},   /* 110 */
    {1,1,1}    /* 111 */
};

static bool valid_vertical(int a, int b, int c)
{
    /* check 3 columns independently */
    for (int col = 0; col < 3; ++col) {
        unsigned char x = BIT[a][col];
        unsigned char y = BIT[b][col];
        unsigned char z = BIT[c][col];
        /* forbidden patterns 0,0,1 and 0,1,0 */
        if ((x == 0 && y == 0 && z == 1) ||
            (x == 0 && y == 1 && z == 0))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* special cases */
    if (n == (idx_t)1) return (seq_t)6;
    if (n == (idx_t)2) return (seq_t)36;

    /* DP over pairs of rows; state index = a*6 + b */
    enum { STATE = 6, PAIR = STATE*STATE };
    seq_t cur[PAIR];
    seq_t nxt[PAIR];

    /* initialize for length 2: every allowed pair is possible */
    for (int i = 0; i < PAIR; ++i)
        cur[i] = (seq_t)1;
    /* iterate from row 3 up to row n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next array */
        for (int i = 0; i < PAIR; ++i)
            nxt[i] = (seq_t)0;

        for (int a_idx = 0; a_idx < STATE; ++a_idx) {
            for (int b_idx = 0; b_idx < STATE; ++b_idx) {
                seq_t val = cur[a_idx*STATE + b_idx];
                if (val == (seq_t)0) continue;
                /* try all possible next rows */
                for (int c_idx = 0; c_idx < STATE; ++c_idx) {
                    if (!valid_vertical(a_idx, b_idx, c_idx))
                        continue;
                    int next_state = b_idx*STATE + c_idx;
                    nxt[next_state] += val;
                }
            }
        }
        /* swap cur and nxt */
        for (int i = 0; i < PAIR; ++i)
            cur[i] = nxt[i];
    }

    /* sum of all states gives a(n) */
    seq_t total = (seq_t)0;
    for (int i = 0; i < PAIR; ++i)
        total += cur[i];
    return total;
}
