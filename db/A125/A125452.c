/*
 * A125452  Number of base 17 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace( M^n ), where M is the 17×17 adjacency matrix
 *            defined by M[i][j] = 1 if |i-j| ≤ 8, otherwise 0. a(0) is
 *            defined to be 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiply two 17×17 matrices: C = A × B */
static void matmul(const seq_t A[17][17], const seq_t B[17][17], seq_t C[17][17])
{
    for (idx_t i = 0; i < 17; ++i) {
        for (idx_t j = 0; j < 17; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 17; ++k)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
}

/* square a matrix in place: M = M × M */
static void square_inplace(seq_t M[17][17])
{
    seq_t tmp[17][17];
    matmul(M, M, tmp);
    for (idx_t i = 0; i < 17; ++i)
        for (idx_t j = 0; j < 17; ++j)
            M[i][j] = tmp[i][j];
}

/* fill the adjacency matrix M with values 0/1 as specified */
static void init_adjacency(seq_t M[17][17])
{
    static bool done = false;
    if (done) return;
    done = true;
    for (idx_t i = 0; i < 17; ++i) {
        for (idx_t j = 0; j < 17; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            M[i][j] = diff <= 8 ? 1 : 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* adjacency matrix */
    static seq_t adj[17][17];
    init_adjacency(adj);

    /* base = adjacency matrix */
    seq_t base[17][17];
    for (idx_t i = 0; i < 17; ++i)
        for (idx_t j = 0; j < 17; ++j)
            base[i][j] = adj[i][j];

    /* result = identity matrix */
    seq_t result[17][17];
    for (idx_t i = 0; i < 17; ++i)
        for (idx_t j = 0; j < 17; ++j)
            result[i][j] = (i == j) ? 1 : 0;

    idx_t power = n;
    while (power) {
        if (power & 1) {
            seq_t tmp[17][17];
            matmul(result, base, tmp);
            for (idx_t i = 0; i < 17; ++i)
                for (idx_t j = 0; j < 17; ++j)
                    result[i][j] = tmp[i][j];
        }
        square_inplace(base);
        power >>= 1;
    }

    /* trace of M^n */
    seq_t sum = 0;
    for (idx_t i = 0; i < 17; ++i)
        sum += result[i][i];

    return sum;
}
