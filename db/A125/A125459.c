/*
 * A125459  Number of base 24 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of base‑24 circular n‑digit numbers whose
 *            adjacent digits differ by at most 8. It equals trace(A^n) where
 *            A is the 24×24 adjacency matrix with A[i][j] = 1 if |i−j| ≤ 8,
 *            else 0. a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void mat_mul(const seq_t (*A)[24], const seq_t (*B)[24], seq_t (*C)[24])
{
    /* C = A * B  (24×24 multiplication) */
    seq_t tmp[24][24];
    /* zero C */
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 24; ++j)
            tmp[i][j] = 0;
    }
    for (int i = 0; i < 24; ++i) {
        for (int k = 0; k < 24; ++k) {
            seq_t aik = A[i][k];
            if (aik == 0) continue;
            for (int j = 0; j < 24; ++j) {
                tmp[i][j] += aik * B[k][j];
            }
        }
    }
    /* copy to C */
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 24; ++j)
            C[i][j] = tmp[i][j];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Build adjacency matrix once */
    static seq_t Adj[24][24];
    static int built = 0;
    if (!built) {
        for (int i = 0; i < 24; ++i) {
            for (int j = 0; j < 24; ++j) {
                if (i > j ? (i - j) <= 8 : (j - i) <= 8)
                    Adj[i][j] = 1;
                else
                    Adj[i][j] = 0;
            }
        }
        built = 1;
    }

    /* res = identity, base = Adj */
    seq_t res[24][24];
    seq_t base[24][24];
    seq_t temp[24][24];

    for (int i = 0; i < 24; ++i)
        for (int j = 0; j < 24; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    for (int i = 0; i < 24; ++i)
        for (int j = 0; j < 24; ++j)
            base[i][j] = Adj[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mul(res, base, temp);
            for (int i = 0; i < 24; ++i)
                for (int j = 0; j < 24; ++j)
                    res[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (!exp) break;
        mat_mul(base, base, temp);
        for (int i = 0; i < 24; ++i)
            for (int j = 0; j < 24; ++j)
                base[i][j] = temp[i][j];
    }

    /* compute trace of res */
    seq_t result = 0;
    for (int i = 0; i < 24; ++i)
        result += res[i][i];
    return result;
}
