/*
 * A124709  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_{j=1}^{16} (1+2*cos(j*π/17))^n for n>0, a(0)=1 This
 *            is equivalent to the trace of the 16×16 adjacency matrix of the
 *            path graph with loops, computed via binary exponentiation
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Base adjacency matrix A of size 16×16 with loops and edges to
       adjacent digits. 0-indexed vertices 0..15. */
    static seq_t A[16][16];
    static int init = 0;
    if (!init) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                if (i == j)
                    A[i][j] = (seq_t)1;
                else if (i + 1 == j || i - 1 == j)
                    A[i][j] = (seq_t)1;
                else
                    A[i][j] = (seq_t)0;
            }
        }
        init = 1;
    }

    /* res = identity matrix, base = A.  All on the stack. */
    seq_t res[16][16];
    seq_t base[16][16];
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j] = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            seq_t temp[16][16];
            for (int i = 0; i < 16; ++i) {
                for (int j = 0; j < 16; ++j) {
                    seq_t sum = (seq_t)0;
                    for (int k = 0; k < 16; ++k)
                        sum = sum + res[i][k] * base[k][j];
                    temp[i][j] = sum;
                }
            }
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 16; ++j)
                    res[i][j] = temp[i][j];
        }

        /* base = base * base */
        seq_t tmp2[16][16];
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                seq_t sum = (seq_t)0;
                for (int k = 0; k < 16; ++k)
                    sum = sum + base[i][k] * base[k][j];
                tmp2[i][j] = sum;
            }
        }
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                base[i][j] = tmp2[i][j];

        exp >>= 1;
    }

    /* Return trace of A^n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 16; ++i)
        total = total + res[i][i];
    return total;
}
