/*
 * A207178  Number of nX4 0..1 arrays avoiding 0 0 0 horizontally and 0 1 0
 *          vertically.
 * https://oeis.org/A207178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207178: number of n×4 binary arrays avoiding horizontal “000” and
   vertical “010”.  The algorithm uses dynamic programming over
   consecutive rows.  For each n≥1 the correct value is returned. */

#include <stdint.h>
#include <stddef.h>

static const uint8_t valid_rows[13] = {
    0b0010, /* 2 */
    0b0011, /* 3 */
    0b0100, /* 4 */
    0b0101, /* 5 */
    0b0110, /* 6 */
    0b0111, /* 7 */
    0b1001, /* 9 */
    0b1010, /* 10 */
    0b1011, /* 11 */
    0b1100, /* 12 */
    0b1101, /* 13 */
    0b1110, /* 14 */
    0b1111  /* 15 */
};

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 13, a(2) = 169 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)169;

    /* dp_prev[i][j] – number of ways for the last two rows equal
       to valid_rows[i] and valid_rows[j] after processing the current
       number of rows.  dp_next will hold the same for the next row. */
    static seq_t dp_prev[13][13];
    static seq_t dp_next[13][13];

    /* initialise for n = 2 : all pairs are allowed, one way each */
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            dp_prev[i][j] = (seq_t)1;

    /* iterate rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dp_next */
        for (int j = 0; j < 13; ++j)
            for (int k = 0; k < 13; ++k)
                dp_next[j][k] = (seq_t)0;

        /* transition */
        for (int i = 0; i < 13; ++i) {
            uint8_t pi = valid_rows[i];
            for (int j = 0; j < 13; ++j) {
                seq_t ways = dp_prev[i][j];
                if (!ways) continue;
                uint8_t pj = valid_rows[j];
                for (int k = 0; k < 13; ++k) {
                    uint8_t pk = valid_rows[k];

                    /* check vertical “010” in each column */
                    int bad = 0;
                    /* bits are positions 3..0, but order does not matter */
                    for (int b = 0; b < 4; ++b) {
                        int bi = (pi >> b) & 1;
                        int bj = (pj >> b) & 1;
                        int bk = (pk >> b) & 1;
                        if (bi == 0 && bj == 1 && bk == 0) {
                            bad = 1; break;
                        }
                    }
                    if (!bad)
                        dp_next[j][k] += ways;
                }
            }
        }

        /* copy dp_next to dp_prev for next iteration */
        for (int j = 0; j < 13; ++j)
            for (int k = 0; k < 13; ++k)
                dp_prev[j][k] = dp_next[j][k];
    }

    /* sum all possibilities for the final two rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            total += dp_prev[i][j];

    return total;
}
