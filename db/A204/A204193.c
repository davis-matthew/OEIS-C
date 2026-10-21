/*
 * A204193  Number of nX4 0..1 arrays with no occurrence of three equal
 *          elements in a row horizontally, vertically or nw-to-se diagonally,
 *          and new values 0..1 introduced in row major order.
 * https://oeis.org/A204193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct n×4 binary arrays (up to a global 0/1
 *            complement) * that contain no three equal entries consecutively
 *            in any row, any column, * or any NW→SE diagonal. The value is
 *            computed by a dynamic‑programming * automaton over the 10
 *            admissible 4‑bit rows. The recurrence is implicit...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define ALLOWED 10

/* The 10 admissible 4‑bit rows (no 000 or 111 as a contiguous triple). */
static const uint8_t rows[ALLOWED] = {
    0b0010, 0b0011,
    0b0100, 0b0101,
    0b0110,
    0b1001,
    0b1010, 0b1011,
    0b1100, 0b1101
};

/* Pre‑computed transition masks:
 * trans[i][j] is a bitmask of indices k such that rows i, rows j, rows k
 * form a valid vertical/diagonal triple. */
static uint16_t trans[ALLOWED][ALLOWED];
static bool trans_inited = false;

/* Check if a triple of rows (i, j, k) satisfies the vertical and diagonal
 * constraints.  Rows i, j, k are indices into rows[]. */
static bool valid_transition(int i, int j, int k)
{
    /* For each column 0..3, disallow a vertical triple of equal bits. */
    for (int col = 0; col < 4; ++col) {
        int a = (rows[i] >> col) & 1U;
        int b = (rows[j] >> col) & 1U;
        int c = (rows[k] >> col) & 1U;
        if (a == b && b == c)
            return false;
    }

    /* Disallow a NW->SE diagonal triple of equal bits.
       Such a triple uses columns col-2 (from rows[i]),
       col-1 (from rows[j]) and col (from rows[k]). */
    for (int col = 2; col < 4; ++col) {
        int a = (rows[i] >> (col - 2)) & 1U;
        int b = (rows[j] >> (col - 1)) & 1U;
        int c = (rows[k] >> col) & 1U;
        if (a == b && b == c)
            return false;
    }
    return true;
}

void init_transitions(void)
{
    if (trans_inited)
        return;
    for (int i = 0; i < ALLOWED; ++i) {
        for (int j = 0; j < ALLOWED; ++j) {
            uint16_t mask = 0;
            for (int k = 0; k < ALLOWED; ++k) {
                if (valid_transition(i, j, k))
                    mask |= (1U << k);
            }
            trans[i][j] = mask;
        }
    }
    trans_inited = true;
}

seq_t a(idx_t n)
{
    init_transitions();

    /* Handle the first two values directly: every admissible row
     * is counted once; for n = 2, every pair of admissible rows
     * is counted once.  The total is divided by 2 to account for
     * global 0/1 complement symmetry. */
    if (n == 1)
        return ((seq_t)ALLOWED) >> 1;  /* 5 */

    if (n == 2)
        return ((seq_t)ALLOWED * ALLOWED) >> 1;  /* 50 */

    /* dp[p1][p2] holds the number of arrays ending with rows
     * rows[p1] and rows[p2] (p2 is the last row).  Initially
     * for length 2 we set all pairs to 1. */
    seq_t dp[ALLOWED][ALLOWED];
    seq_t ndp[ALLOWED][ALLOWED];
    for (int i = 0; i < ALLOWED; ++i)
        for (int j = 0; j < ALLOWED; ++j)
            dp[i][j] = (seq_t)1;

    /* Process rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear ndp */
        for (int i = 0; i < ALLOWED; ++i)
            for (int j = 0; j < ALLOWED; ++j)
                ndp[i][j] = (seq_t)0;

        for (int p1 = 0; p1 < ALLOWED; ++p1) {
            for (int p2 = 0; p2 < ALLOWED; ++p2) {
                seq_t cur = dp[p1][p2];
                if (cur == 0)
                    continue;
                uint16_t mask = trans[p1][p2];
                while (mask) {
                    int r = __builtin_ctz(mask);   /* index of lowest set bit */
                    ndp[p2][r] += cur;
                    mask &= mask - 1;              /* clear lowest bit */
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < ALLOWED; ++i)
            for (int j = 0; j < ALLOWED; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all endings to obtain total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < ALLOWED; ++i)
        for (int j = 0; j < ALLOWED; ++j)
            total += dp[i][j];

    /* Divide by 2 for complement symmetry. */
    return total >> 1;
}
