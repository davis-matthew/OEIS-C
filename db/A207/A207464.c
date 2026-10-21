/*
 * A207464  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Implements a(n) via dynamic programming.
   a(n) = number of n×5 0..1 arrays avoiding
   horizontal patterns 000 and 011 and vertical patterns 010 and 101. */

static const unsigned char ALLOWED[13] = {
    0b00100, 0b00101, 0b01001, 0b01010,
    0b10010, 0b10100, 0b10101, 0b11001,
    0b11010, 0b11100, 0b11101, 0b11110, 0b11111
};

static unsigned short NEXT_MASK[13][13];
static int MASKS_READY = 0;

static void init_masks(void)
{
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            unsigned short m = 0;
            for (int k = 0; k < 13; ++k) {
                int ok = 1;
                for (int col = 0; col < 5; ++col) {
                    int a = (ALLOWED[i] >> col) & 1;
                    int b = (ALLOWED[j] >> col) & 1;
                    int c = (ALLOWED[k] >> col) & 1;
                    if ((a == 0 && b == 1 && c == 0) ||
                        (a == 1 && b == 0 && c == 1)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) m |= (1u << k);
            }
            NEXT_MASK[i][j] = m;
        }
    }
    MASKS_READY = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)169;

    if (!MASKS_READY) init_masks();

    seq_t cur[13][13];
    seq_t nxt[13][13];
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j) {
            cur[i][j] = 1;   /* one way to choose row1=i, row2=j */
            nxt[i][j] = 0;   /* ensure nxt starts zeroed */
        }

    for (idx_t row = 3; row <= n; ++row) {
        for (int a = 0; a < 13; ++a) {
            for (int b = 0; b < 13; ++b) {
                seq_t val = cur[a][b];
                if (val == 0) continue;
                unsigned short mask = NEXT_MASK[a][b];
                for (int k = 0; k < 13; ++k) {
                    if (mask & (1u << k))
                        nxt[b][k] += val;
                }
            }
        }
        for (int i = 0; i < 13; ++i)
            for (int j = 0; j < 13; ++j) {
                cur[i][j] = nxt[i][j];
                nxt[i][j] = 0;   /* reset for next iteration */
            }
    }

    seq_t total = 0;
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            total += cur[i][j];

    return total;
}
